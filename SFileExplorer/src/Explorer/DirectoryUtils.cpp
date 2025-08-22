#include "DirectoryUtils.h"
#include <windows.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <shlobj.h>
std::string WideToUTF8(const std::wstring& wstr) {
	if (wstr.empty()) return {};

	int size_needed = WideCharToMultiByte(
		CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr
	);
	std::string result(size_needed - 1, 0); //Exclude null terminator
	WideCharToMultiByte(
		CP_UTF8, 0, wstr.c_str(), -1, result.data(), size_needed, nullptr, nullptr
	);
	return result;
}

bool Explorer::IsDirectory(const Explorer::Path& path) {
	return (std::filesystem::exists(path) && std::filesystem::is_directory(path));
}

bool CanAccessDirectory(const Explorer::Path& entry) {
	DWORD attributes = GetFileAttributesW(entry.c_str());
	if (attributes == INVALID_FILE_ATTRIBUTES) return false;

	// Try opening the file or directory
	HANDLE h = CreateFileW(
		entry.c_str(),
		FILE_READ_ATTRIBUTES, // Works for both files and folders
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		(attributes & FILE_ATTRIBUTE_DIRECTORY) ? FILE_FLAG_BACKUP_SEMANTICS : 0,
		NULL
	);

	if (h == INVALID_HANDLE_VALUE) return false;
	CloseHandle(h);
	return true;
}

Explorer::Directory::~Directory() {
	m_files.clear();
}

bool IsVisibleInExplorer(const Explorer::Path& p) {
	DWORD attrs = GetFileAttributesW(p.c_str());
	if (attrs == INVALID_FILE_ATTRIBUTES) return false;

	// Hide junctions and other reparse points
	if (attrs & FILE_ATTRIBUTE_REPARSE_POINT)
		return false;

	// Show all directories (except reparse points)
	if (attrs & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	// Hide files marked both SYSTEM and HIDDEN
	if ((attrs & FILE_ATTRIBUTE_SYSTEM) && (attrs & FILE_ATTRIBUTE_HIDDEN))
		return false;

	return true;
}

std::string GetFileExtension(const Explorer::Path& entry) {
	std::string extension = entry.extension().string();
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	return extension;
}


std::string GetFileTypeDescription(const std::filesystem::path& path) {
	if (std::filesystem::is_directory(path)) return "Folder";
	SHFILEINFOW info = {};
	SHGetFileInfoW(
		path.c_str(),
		FILE_ATTRIBUTE_NORMAL,
		&info,
		sizeof(info),
		SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES
	);
	return WideToUTF8(info.szTypeName);
}

void Explorer::OpenFile(const Explorer::Path& path) {
	HINSTANCE result = ShellExecuteW(
		NULL,
		L"open",
		path.c_str(),
		NULL,
		NULL,
		SW_SHOWNORMAL
	);

	if ((INT_PTR)result <= 32) {

	}
}

Explorer::Path Explorer::GetUserDirectory() {
	PWSTR widePath = nullptr;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Profile, 0, nullptr, &widePath))) {
		std::filesystem::path userDir(widePath); // Safe conversion
		CoTaskMemFree(widePath);
		return userDir;
	}
	return {};
}


Explorer::Directory::Directory(const Explorer::Path& path) : path(path) {
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		auto entryPath = entry.path();
		if (CanAccessDirectory(entryPath) && IsVisibleInExplorer(entryPath)) {
			m_files.push_back({
				entryPath,
				entryPath.filename().string(),
				GetFileExtension(entryPath),
				GetFileTypeDescription(entryPath),
				IsDirectory(entryPath)
			});
		}
	}

	M_Sort();
}

void Explorer::Directory::Reload() {
	m_files.clear();

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		auto entryPath = entry.path();
		if (CanAccessDirectory(entryPath) && IsVisibleInExplorer(entryPath)) {
			m_files.push_back({
				entryPath,
				entryPath.filename().string(),
				GetFileExtension(entryPath),
				GetFileTypeDescription(entryPath),
				IsDirectory(entryPath)
			});
		}
	}
	
	M_Sort();
}

void Explorer::Directory::M_Sort() {
	switch (m_type) {
		case SortType::Alpha:
			std::sort(begin(), end(), [](const File& a, const File& b) {
				// Folders first
				if (a.isDirectory != b.isDirectory)
					return a.isDirectory > b.isDirectory;

				// Compare extensions (case-insensitive)
				std::wstring extA = a.path.extension().wstring();
				std::wstring extB = b.path.extension().wstring();
				std::transform(extA.begin(), extA.end(), extA.begin(), ::towlower);
				std::transform(extB.begin(), extB.end(), extB.begin(), ::towlower);
				return extA < extB;
			});	
		break;

		case SortType::ReverseAlpha:
			std::sort(begin(), end(), [](const File& a, const File& b) {
				// Folders first
				if (a.isDirectory != b.isDirectory)
					return a.isDirectory < b.isDirectory;

				// Compare extensions (case-insensitive)
				std::wstring extA = a.path.extension().wstring();
				std::wstring extB = b.path.extension().wstring();
				std::transform(extA.begin(), extA.end(), extA.begin(), ::towlower);
				std::transform(extB.begin(), extB.end(), extB.begin(), ::towlower);
				if (extA != extB)
					return extA > extB; // Descending

				// Secondary sort by name
				return a.name > b.name;
			});
		break;
	}
}
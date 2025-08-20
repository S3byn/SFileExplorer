#include "DirectoryUtils.h"
#include <Windows.h>

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

Explorer::Directory::Directory(const Explorer::Path& path) : path(path) {
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		auto entryPath = entry.path();
		if (CanAccessDirectory(entryPath) && IsVisibleInExplorer(entryPath)) {
			m_files.push_back({
				entryPath,
				entryPath.filename().string(),
				IsDirectory(entryPath)
			});
		}
	}
}

void Explorer::Directory::Reload() {
	m_files.clear();

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		auto entryPath = entry.path();
		if (CanAccessDirectory(entryPath) && IsVisibleInExplorer(entryPath)) {
			m_files.push_back({
				entryPath,
				entryPath.filename().string(),
				IsDirectory(entryPath)
			});
		}
	}
}
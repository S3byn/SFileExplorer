#include "DirectoryUtils.h"

bool Explorer::IsDirectory(Explorer::Path path) {
	return (std::filesystem::exists(path) && std::filesystem::is_directory(path));
}

Explorer::Directory::Directory(const Path& path) : m_path(path) {
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		auto entryPath = entry.path();

		m_files.push_back({
			entryPath,
			entryPath.filename().string(),
			IsDirectory(entryPath)
		});
	}
}


Explorer::Directory::~Directory() {
	m_files.clear();
}

void Explorer::Directory::ReloadDirectory(const Path& path) {
	m_path = path;
	m_files.clear();

	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		auto entryPath = entry.path();

		m_files.push_back({
			entryPath,
			entryPath.filename().string(),
			IsDirectory(entryPath)
		});
	}
}
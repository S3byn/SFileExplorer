#pragma once
#include <filesystem>
#include <vector>

namespace Explorer {
	using Path = std::filesystem::path;

	struct File {
		Path path;
		std::string name;
		bool isDirectory;
	};

	bool IsDirectory(Path path);

	class Directory {
	public:
		Directory(const Path& path);
		~Directory();

		void ReloadDirectory(const Path& path);
		
		const Path& GetPath() const { return m_path; }

		std::vector<File>::iterator begin() { return m_files.begin(); }
		std::vector<File>::iterator end() { return m_files.end(); }

		std::vector<File>::const_iterator begin() const { return m_files.begin(); }
		std::vector<File>::const_iterator end() const { return m_files.end(); }
		
	private:
		std::vector<File> m_files;
		Path m_path;
	};



}



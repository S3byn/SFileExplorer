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

	bool IsDirectory(const Path& path);

	void OpenFile(const Path& path);

	class Directory {
	public:
		Directory(const Path& path);
		~Directory();

		void Reload();
		
		std::vector<File>::iterator begin() { return m_files.begin(); }
		std::vector<File>::iterator end() { return m_files.end(); }

		std::vector<File>::const_iterator begin() const { return m_files.begin(); }
		std::vector<File>::const_iterator end() const { return m_files.end(); }
		
		Path path;
	private:
		std::vector<File> m_files;
	};



}



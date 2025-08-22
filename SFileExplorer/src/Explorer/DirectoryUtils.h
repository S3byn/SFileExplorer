#pragma once
#include <filesystem>
#include <vector>

namespace Explorer {
	// Alias for std::filesytem::path
	using Path = std::filesystem::path;

	// Basic File data structure
	struct File {
		Path path;
		std::string name;
		std::string extension;
		std::string typeDesc;
		bool isDirectory;
	};

	bool IsDirectory(const Path& path);

	void OpenFile(const Path& path);

	Path GetUserDirectory();

	// Directory organization stuff
	enum class SortType {
		Alpha, ReverseAlpha
	};

	class Directory {
	public:
		Directory(const Path& path);
		~Directory();

		void Reload();
		
		std::vector<File>::iterator begin() { return m_files.begin(); }
		std::vector<File>::iterator end() { return m_files.end(); }

		std::vector<File>::const_iterator begin() const { return m_files.begin(); }
		std::vector<File>::const_iterator end() const { return m_files.end(); }
		
		void SetSort(SortType type) { m_type = type; }

		Path path;
	private:
		void M_Sort();

		std::vector<File> m_files;
		SortType m_type = SortType::Alpha;
	};



}



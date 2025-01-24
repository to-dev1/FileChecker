#include "FileUtility.h"

void getAllFiles(const std::filesystem::path& path, std::vector<std::filesystem::directory_entry>& items, const bool recursive)
{
	auto root = std::filesystem::directory_entry(path);
	if (root.exists())
	{
		//std::cout << root << std::endl;

		for (const auto& item : std::filesystem::directory_iterator(path))
		{
			items.push_back(item);

			if (recursive && item.is_directory())
			{
				getAllFiles(item.path(), items, recursive);
			}
		}
	}
}

void getFileLines(const std::filesystem::path& path, std::vector<std::string>& lines)
{
	std::ifstream file(path);
	std::string line;
	while (std::getline(file, line))
	{
		lines.push_back(line);
	}
}
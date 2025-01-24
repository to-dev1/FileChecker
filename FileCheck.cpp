#include "FileCheck.h"

void getFileInfo(const std::filesystem::directory_entry& item, std::ostream& output, Console* console)
{
	auto time = std::chrono::clock_cast<std::chrono::system_clock>(item.last_write_time());
	auto const timeZone = std::chrono::locate_zone("Etc/GMT-3");
	output << console->addSpaces(std::format("{:%FT%T%Z}", std::chrono::zoned_time(timeZone, floor<std::chrono::milliseconds>(time))), 35) << console->addSpaces(std::to_string(item.file_size()), 20) << item.path().string() << std::endl;
}

bool getFiles(std::vector<std::filesystem::directory_entry>& files, Command& cmd, std::vector<Parameter>& parameters, std::ostream& output, Console* console)
{
	bool recursive = cmd.checkSetting(parameters, "r");

	if (parameters.size() == 0)
	{
		getAllFiles(std::filesystem::current_path(), files, recursive);
	}
	else
	{
		std::filesystem::path path = parameters[0].word;

		if (std::filesystem::directory_entry(path).exists())
		{
			getAllFiles(path, files, recursive);
		}
		else
		{
			std::cout << "The path does not exist: " << path << std::endl;
		}
	}

	return recursive;
}

void checkFile(const std::filesystem::path& path, const int rate, std::ostream& output, Console* console)
{
	//Get lines from the file
	std::vector<std::string> lines;
	getFileLines(path, lines);

	int count = lines.size() / rate + 1;
	count = std::min(count, static_cast<int>(lines.size()));

	for (int i = 0; i < 75; i++)
	{
		output << "- ";
	}
	output << std::endl;

	if (lines.size() != 0)
	{
		/*
		for (int i = 0; i < count; i++)
		{
			int index = rand() % lines.size();
			output << " > Line: " << console->addSpaces(std::to_string(index), 6) << "Characters: " << console->addSpaces(std::to_string(lines[index].size()), 6) << lines[index] << std::endl;
		}
		*/

		float range = static_cast<float>(lines.size()) / static_cast<float>(count);
		for (int i = 0; i < count; i++)
		{
			float random = static_cast<float>(rand() % 1000) * 0.001f;
			int index = static_cast<int>((static_cast<float>(i) + random) * range);
			index = std::min(index, static_cast<int>(lines.size() - 1));
			//std::cout << index << ", " << random << ", " << range << std::endl;
			output << " > Line: " << console->addSpaces(std::to_string(index), 6) << "Characters: " << console->addSpaces(std::to_string(lines[index].size()), 6) << lines[index] << std::endl;
		}
	}
	else
	{
		output << "Empty file" << std::endl;
	}

	for (int i = 0; i < 75; i++)
	{
		output << "- ";
	}
	output << std::endl;
}

void FilesCommand::run(std::vector<Parameter>& parameters, std::ostream& output, Console* console)
{
	std::vector<std::filesystem::directory_entry> files;

	std::vector<std::string> fileTypes = { ".txt", ".cpp", ".h", ".glsl" };
	if (checkSetting(parameters, "a")) fileTypes.clear();

	bool recursive = getFiles(files, *this, parameters, output, console);

	int count = 0;
	if (files.size() != 0)
	{
		output << console->addSpaces("Last modified", 35) << console->addSpaces("Size in bytes", 20) << std::endl;

		for (auto it : files)
		{
			bool add = fileTypes.size() == 0;

			if (!add)
			{
				for (auto jt : fileTypes)
				{
					if (it.path().extension() == jt)
					{
						add = true;
						break;
					}
				}
			}

			if (add)
			{
				getFileInfo(it, output, console);
				count += 1;
			}
		}
	}

	output << "Files: " << count << "/" << files.size() << std::endl;
}

void FileReaderCommand::run(std::vector<Parameter>& parameters, std::ostream& output, Console* console)
{
	std::vector<std::filesystem::directory_entry> files;
	bool recursive = getFiles(files, *this, parameters, output, console);
	if (parameters.size() != 0)
	{
		parameters.erase(parameters.begin());
	}

	runReader(parameters, output, console, files);
}

void FileReaderCommand::addInfo(std::ostream& output)
{
	output << "Parameters:\n";
	fileReader.getInfo(output);
	addReaderInfo(output);
}

void CheckCommand::runReader(std::vector<Parameter>& parameters, std::ostream& output, Console* console, std::vector<std::filesystem::directory_entry>& files)
{
	std::vector<std::filesystem::path> fileTypes = { ".txt", ".cpp", ".h", ".glsl" };

	int seed = 0;
	int checkRate = 100; //100
	int lineCheckRate = 100; //1000

	seedReader.read(seed, parameters);
	rateReader.read(checkRate, parameters);
	lineRateReader.read(lineCheckRate, parameters);

	//Initialize random
	srand(0);

	int count = 0;
	if (files.size() != 0)
	{
		std::stringstream st;
		st << console->addSpaces("Last modified", 35) << console->addSpaces("Size in bytes", 20) << std::endl;
		std::string infoColumns = st.str();
		output << infoColumns;

		for (auto it : files)
		{
			bool add = vecUtil::contains(fileTypes, it.path().extension());

			if (add && rand() % checkRate == 0)
			{
				getFileInfo(it, output, console);
				checkFile(it.path(), lineCheckRate, output, console); //1000
				count += 1;
			}
		}
	}

	output << "Files: " << count << "/" << files.size() << std::endl;
}

void CheckCommand::addReaderInfo(std::ostream& output)
{
	seedReader.getInfo(output);
	rateReader.getInfo(output);
	lineRateReader.getInfo(output);
}

void CountCommand::runReader(std::vector<Parameter>& parameters, std::ostream& output, Console* console, std::vector<std::filesystem::directory_entry>& files)
{
	std::vector<std::string> fileTypes;
	std::string tp;
	while (fileTypeReader.read(tp, parameters))
	{
		if (tp[0] != '.') tp = "." + tp;
		fileTypes.push_back(tp);
	}

	output << fileTypes.size() << " file types to count: ";
	for (auto it : fileTypes)
	{
		output << it << " ";
	}
	output << std::endl;

	int count = 0;
	int lineCount = 0;
	int charCount = 0;

	for (const auto& it : files)
	{
		if (vecUtil::contains(fileTypes, it.path().extension().string()))
		{
			std::vector<std::string> lines;
			getFileLines(it, lines);
			count++;
			lineCount += lines.size();
			for (const auto& jt : lines)
			{
				charCount += jt.size();
			}
		}
	}

	output << console->addSpaces("Files", 20) << count << std::endl;
	output << console->addSpaces("Lines", 20) << lineCount << std::endl;
	output << console->addSpaces("Characters", 20) << charCount << std::endl;
}

void CountCommand::addReaderInfo(std::ostream& output)
{
	fileTypeReader.getInfo(output);
	output << "... other file types to count\n";
}
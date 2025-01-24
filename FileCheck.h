#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>
#include <fstream>

#include "Command.h"
#include "FileUtility.h"

void getFileInfo(const std::filesystem::directory_entry& item, std::ostream& output, Console* console);

bool getFiles(std::vector<std::filesystem::directory_entry>& files, Command& cmd, std::vector<Parameter>& parameters, std::ostream& output, Console* console);

void checkFile(const std::filesystem::path& path, const int rate, std::ostream& output, Console* console);

namespace vecUtil
{
	template<typename T>
	size_t indexOf(const std::vector<T>& vec, const T& item)
	{
		auto len = vec.size();
		for (size_t i = 0; i < len; i++)
		{
			if (vec[i] == item) return i;
		}

		return std::numeric_limits<size_t>::max();
	}

	template<typename T>
	bool contains(const std::vector<T>& vec, const T& item)
	{
		size_t idx = indexOf(vec, item);
		if (idx != std::numeric_limits<size_t>::max())
		{
			return true;
		}

		return false;
	}
}

class FilesCommand : public Command
{
public:
	FilesCommand() : Command("files", "Recursively list all files in the target directory") {};

	virtual FilesCommand* clone() const
	{
		return new FilesCommand(*this);
	}

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console);
};

class FileReaderCommand : public Command
{
public:
	ReaderStr fileReader;

	FileReaderCommand(const std::string& c = "", const std::string & d = "") : Command(c, d),
		fileReader("Path to be read, reads working directory if argument was not given")
	{};

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console);
	virtual void runReader(std::vector<Parameter>& parameters, std::ostream& output, Console* console, std::vector<std::filesystem::directory_entry>& files) = 0;

	virtual void addInfo(std::ostream& output);
	virtual void addReaderInfo(std::ostream& output) {};
};

class CheckCommand : public FileReaderCommand
{
public:
	ReaderInt seedReader;
	ReaderInt rateReader;
	ReaderInt lineRateReader;

	CheckCommand() : FileReaderCommand("check", "Randomly print lines from the files in the target directory"),
		seedReader("Seed to be used by the random generator to sample the directory"), 
		rateReader("1 out of this many files are checked"), 
		lineRateReader("1 out of this many lines are checked") 
	{};

	virtual CheckCommand* clone() const
	{
		return new CheckCommand(*this);
	}

	virtual void runReader(std::vector<Parameter>& parameters, std::ostream& output, Console* console, std::vector<std::filesystem::directory_entry>& files);

	virtual void addReaderInfo(std::ostream& output);
};

class CountCommand : public FileReaderCommand
{
public:
	ReaderStr fileReader;
	ReaderStr fileTypeReader;

	CountCommand() : FileReaderCommand("count", "Count number of files, lines and characters"),
		fileTypeReader("File type counted")
	{};

	virtual CountCommand* clone() const
	{
		return new CountCommand(*this);
	}

	virtual void runReader(std::vector<Parameter>& parameters, std::ostream& output, Console* console, std::vector<std::filesystem::directory_entry>& files);

	virtual void addReaderInfo(std::ostream& output);
};
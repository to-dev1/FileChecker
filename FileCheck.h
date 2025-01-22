#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>
#include <fstream>

#include "Command.h"

//Recursively get all files
void getAllFiles(const std::filesystem::path& path, std::vector<std::filesystem::directory_entry>& items, const bool recursive);

void getFileInfo(const std::filesystem::directory_entry& item, std::ostream& output, Console* console);

bool getFiles(std::vector<std::filesystem::directory_entry>& files, Command& cmd, std::vector<Parameter>& parameters, std::ostream& output, Console* console);

void checkFile(const std::filesystem::path& path, const int rate, std::ostream& output, Console* console);

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

class CheckCommand : public Command
{
public:
	CheckCommand() : Command("check", "Randomly print lines from the files in the target directory") {};

	virtual CheckCommand* clone() const
	{
		return new CheckCommand(*this);
	}

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console);
};
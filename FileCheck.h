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
	ReaderStr fileReader;
	ReaderInt seedReader;
	ReaderInt rateReader;
	ReaderInt lineRateReader;

	CheckCommand() : Command("check", "Randomly print lines from the files in the target directory"), 
		fileReader("Path to be checked, checks working directory if argument was not given"),
		seedReader("Seed to be used by the random generator to sample the directory"), 
		rateReader("1 out of this many files are checked"), 
		lineRateReader("1 out of this many lines are checked") 
	{};

	virtual CheckCommand* clone() const
	{
		return new CheckCommand(*this);
	}

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console);

	virtual void addInfo(std::ostream& output);
};
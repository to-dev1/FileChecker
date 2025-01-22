#pragma once

#include <string>
#include <vector>
#include <iostream>

class Console;

class Parameter
{
public:
	std::string word;

	Parameter(const std::string& w) : word(w) {};
};

class Command
{
public:
	//Name to call command with
	std::string name;
	//Command description
	std::string desc;

	Command(const std::string& n, const std::string& d) : name(n), desc(d) {};

	bool checkSetting(std::vector<Parameter>& parameters, const std::string& setting);

	virtual Command* clone() const = 0;

	void execute(std::vector<Parameter>& parameters, std::ostream& output, Console* console);

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console) = 0;
};

class Console
{
public:
	std::vector<Command*> commands;
	bool quit;

	Console(const std::vector<Command*>& cmd) : commands(cmd), quit(false) {};
	Console(const Console& c);
	Console& operator=(const Console& c);

	~Console()
	{
		for (int i = 0; i < commands.size(); i++)
		{
			delete commands[i];
		}
	}

	std::string addSpaces(const std::string& s, const int spaces);

	//Returns command index by name, -1 if not found
	int findCommand(const std::string& cmd);

	void parseToWords(const std::string& str, std::vector<std::string>& words);

	void execute(const std::string& cmd, std::ostream& output);
};
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <functional>

class Console;

class Parameter
{
public:
	std::string word;

	Parameter(const std::string& w) : word(w) {};
};

template<typename... Types>
class ParameterReader
{
public:
	std::string name;
	std::string desc;
	bool required;

	ParameterReader(bool r = false, const std::string& n = "", const std::string& d = "") : name(n), desc(d), required(r) {};

	virtual void readParameter(Types&&... mods, const Parameter& param) = 0;

	bool read(Types&... mods, std::vector<Parameter>& parameters)
	{
		if (parameters.size() != 0)
		{
			readParameter(std::forward<Types>(mods)..., parameters[0]);
			parameters.erase(parameters.begin());
			return true;
		}

		return false;
	}

	void getInfo(std::ostream& output)
	{
		output << "[" << name << "] ";
		if (required)
		{
			output << "(Required)";
		}
		else
		{
			output << "(Optional)";
		}
		output << " - " << desc << std::endl;
	}
};
/*
class ReaderTest : public ParameterReader<int, std::string>
{
public:
	ReaderTest() : ParameterReader<int, std::string>("reader") {};

	void readParameter(int&& a, std::string&& b)
	{
		std::cout << a << ", " << b << std::endl;
		a = 100;
		b = "def";
		return true;
	}
};
*/
class ReaderInt : public ParameterReader<int>
{
public:
	ReaderInt(const std::string& d = "", bool r = false) : ParameterReader<int>(r, "INTEGER", d) {};

	void readParameter(int&& a, const Parameter& param)
	{
		a = std::stoi(param.word);
	}
};

class ReaderStr : public ParameterReader<std::string>
{
public:
	ReaderStr(const std::string& d = "", bool r = false) : ParameterReader<std::string>(r, "STRING", d) {};

	void readParameter(std::string&& a, const Parameter& param)
	{
		a = param.word;
	}
};

class Command
{
public:
	//Name to call command with
	std::string name;
	//Command description
	std::string desc;
	//Block execution of the next command
	bool blockNext;

	Command(const std::string& n, const std::string& d, bool b = false) : name(n), desc(d), blockNext(b) {};

	bool checkSetting(std::vector<Parameter>& parameters, const std::string& setting);

	template<typename Func>
	void nextParameter(Func func, std::vector<Parameter>& parameters)
	{
		if (parameters.size() != 0)
		{
			func(parameters[0].word);
			parameters.erase(parameters.begin());
		}
	}

	virtual Command* clone() const = 0;

	void execute(std::vector<Parameter>& parameters, std::ostream& output, Console* console);

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console) = 0;

	void getInfo(std::ostream& output)
	{
		output << "<" << name << "> - " << desc << std::endl;
		addInfo(output);
		if (blockNext)
		{
			output << "{!} Blocks the next command from executing" << std::endl;
		}
	}

	virtual void addInfo(std::ostream& output) {};
};

class Console
{
public:
	std::vector<Command*> commands;
	int executedCommandCount;
	bool quit;

	std::string cmdSymbol;
	std::string startMessage;

	Console(const std::vector<Command*>& cmd, const std::string& s = ">", const std::string& m = "") : commands(cmd), cmdSymbol(s), startMessage(m), quit(false), executedCommandCount(0) {};
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

	void runConsole(std::istream& input, std::ostream& output);
};
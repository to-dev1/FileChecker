#pragma once

#include "Command.h"

class HelloCommand : public Command
{
public:
	HelloCommand() : Command("hello", "Prints a greeting to the console") {};

	virtual HelloCommand* clone() const
	{
		return new HelloCommand(*this);
	}

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console);
};

class HelpCommand : public Command
{
public:
	ReaderStr commandReader;

	HelpCommand() : Command("help", "This command, displays all available commands and their use cases", true), commandReader("Command to get information about") {};

	virtual HelpCommand* clone() const
	{
		return new HelpCommand(*this);
	}

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console);

	virtual void addInfo(std::ostream& output);
};

class QuitCommand : public Command
{
public:
	QuitCommand() : Command("quit", "Exit program") {};

	virtual QuitCommand* clone() const
	{
		return new QuitCommand(*this);
	}

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console);
};
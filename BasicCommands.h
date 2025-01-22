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
	HelpCommand() : Command("help", "This command, displays all available commands and their use cases") {};

	virtual HelpCommand* clone() const
	{
		return new HelpCommand(*this);
	}

	virtual void run(std::vector<Parameter>& parameters, std::ostream& output, Console* console);
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
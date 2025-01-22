#include "BasicCommands.h"

void HelloCommand::run(std::vector<Parameter>& parameters, std::ostream& output, Console* console)
{
	if (parameters.size() == 0)
	{
		output << "Hello there!" << std::endl;
	}
	else
	{
		output << "Hello there ";

		for (int i = 0; i < parameters.size(); i++)
		{
			output << parameters[i].word;
			if (i != parameters.size() - 1)
			{
				if (i == parameters.size() - 2)
				{
					output << " and ";
				}
				else
				{
					output << ", ";
				}
			}
		}

		output << "!" << std::endl;
	}
}

void HelpCommand::run(std::vector<Parameter>& parameters, std::ostream& output, Console* console)
{
	std::cout << console->addSpaces("# Command", 15) << "# Description" << std::endl;

	for (int i = 0; i < console->commands.size(); i++)
	{
		const Command* cmd = console->commands[i];

		std::cout << console->addSpaces(cmd->name, 15) << cmd->desc << std::endl;
	}
}

void QuitCommand::run(std::vector<Parameter>& parameters, std::ostream& output, Console* console)
{
	std::cout << "Quit program" << std::endl;

	console->quit = true;
}
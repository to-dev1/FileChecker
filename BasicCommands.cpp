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
	std::string command = "";

	//std::cout << "param: " << parameters.size() << std::endl;

	if (commandReader.read(command, parameters))
	{
		const int cmdIdx = console->findCommand(command);

		if (cmdIdx != -1)
		{
			console->commands[cmdIdx]->getInfo(output);
		}
		else
		{
			output << "Command not found, use help without parameters to see all available commands" << std::endl;
		}
	}
	else
	{
		output << console->addSpaces("# Command", 15) << "# Description" << std::endl;

		for (int i = 0; i < console->commands.size(); i++)
		{
			const Command* cmd = console->commands[i];

			output << console->addSpaces(cmd->name, 15) << cmd->desc << std::endl;
		}

		output << "\nWhen running a command, file checker displays its interpreation of the command in this format:\n<Command 1> [Argument 1] [Argument 2] <Command 2> [Argument 1]" << std::endl;
	}
}

void HelpCommand::addInfo(std::ostream& output)
{
	output << "Parameters:\n";
	commandReader.getInfo(output);
}

void QuitCommand::run(std::vector<Parameter>& parameters, std::ostream& output, Console* console)
{
	output << "Quit program" << std::endl;

	console->quit = true;
}
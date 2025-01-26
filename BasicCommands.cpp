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
		output << console->commands.size() << " available commands" << std::endl;
		output << console->addSpaces("# Command", 15) << "# Description" << std::endl;

		for (int i = 0; i < console->commands.size(); i++)
		{
			const Command* cmd = console->commands[i];

			output << console->addSpaces(cmd->name, 15) << cmd->desc << std::endl;
		}

		output << "\nWhen running a command, file checker displays its interpretation of the command in this format:\n<Command 1> [Argument 1] [Argument 2] <Command 2> [Argument 1]" << std::endl;
		output << "\nTo prevent the command interpreter from splitting a text into multiple commands at each space, write the command as follows:\ncommand \"argument one\"" << std::endl;
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

std::string parsePattern(const std::string& found, const std::vector<std::string>& replace, std::ostream& output)
{
	std::string pattern = "";

	bool getIndex = false;
	std::string indexStr = "";
	for (int i = 0; i < found.size(); i++)
	{
		char c = found[i];

		if (c == '{')
		{
			getIndex = true;
		}
		else if (c == '}' && getIndex)
		{
			getIndex = false;
			char* ptr;
			int index = static_cast<int>(strtol(indexStr.c_str(), &ptr, 10));
			if (*ptr)
			{
				output << "{" << indexStr << "} could not be converted to an integer" << std::endl;
			}
			else if(index >= 0 && index < replace.size())
			{
				pattern.append('"' + replace[index] + '"');
			}
			else
			{
				output << "{" << index << "} out of range" << std::endl;
			}
		}
		else if (getIndex)
		{
			indexStr += c;
		}
		else
		{
			pattern += c;
		}
	}

	return pattern;
}

void PatternCommand::run(std::vector<Parameter>& parameters, std::ostream& output, Console* console)
{
	std::string patternPath = "";

	if (pathReader.read(patternPath, parameters))
	{
		std::filesystem::path path = std::filesystem::current_path().append(patternPath);
		//output << "Pattern path: " << path << std::endl;
		if (std::filesystem::directory_entry(path).exists())
		{
			std::vector<std::string> lines;
			getFileLines(path, lines);

			std::string pattern = "";
			for (const auto& it : lines)
			{
				pattern.append(it);
				if (pattern[pattern.size() - 1] != ' ')
				{
					pattern += ' ';
				}
			}

			output << "Found pattern: " << pattern << std::endl;

			std::vector<std::string> replace;
			std::string rep;
			while (patternReader.read(rep, parameters))
			{
				replace.push_back(rep);
			}

			for (int i = 0; i < replace.size(); i++)
			{
				output << "{" << i << "} -> " << replace[i] << std::endl;
			}

			//Parse found pattern to replace
			std::string runPattern = parsePattern(pattern, replace, output);

			output << "Final pattern to run: " << runPattern << std::endl;

			console->execute(runPattern, output);
		}
		else
		{
			output << "The path " << path << " does not exist";
		}
	}
	else
	{
		output << "No path to pattern given" << std::endl;
	}
}

void PatternCommand::addInfo(std::ostream& output)
{
	output << "Parameters:\n";
	pathReader.getInfo(output);
	patternReader.getInfo(output);
	output << "... other patterns to replace\n";
}

#include "Command.h"

bool Command::checkSetting(std::vector<Parameter>& parameters, const std::string& setting)
{
	std::string target = "-" + setting;
	
	for (int i = 0; i < parameters.size(); i++)
	{
		if (parameters[i].word == target)
		{
			parameters.erase(parameters.begin() + i);
			return true;
		}
	}

	return false;
}

void Command::execute(std::vector<Parameter>& parameters, std::ostream& output, Console* console)
{
	output << "<" << name << ">";

	for (int i = 0; i < parameters.size(); i++)
	{
		output << " [" << parameters[i].word << "]";
	}

	output << std::endl;

	//Run the implementation of this command
	run(parameters, output, console);

	//Increment
	console->executedCommandCount++;
}

Console::Console(const Console& c)
{
	quit = c.quit;
	executedCommandCount = c.executedCommandCount;
	cmdSymbol = c.cmdSymbol;
	startMessage = c.startMessage;

	for (int i = 0; i < c.commands.size(); i++)
	{
		commands.push_back(c.commands[i]->clone());
	}
}

Console& Console::operator=(const Console& c)
{
	//Delete current commands
	for (int i = 0; i < commands.size(); i++)
	{
		delete commands[i];
	}
	commands.clear();

	quit = c.quit;
	executedCommandCount = c.executedCommandCount;
	cmdSymbol = c.cmdSymbol;
	startMessage = c.startMessage;

	for (int i = 0; i < c.commands.size(); i++)
	{
		commands.push_back(c.commands[i]->clone());
	}
}

std::string Console::addSpaces(const std::string& s, const int spaces)
{
	std::string str = s;
	int c = spaces - str.size();
	if (c > 0)
	{
		for (int i = 0; i < c; i++)
		{
			str += ' ';
		}
	}

	return str;
}

int Console::findCommand(const std::string& cmd)
{
	for (int i = 0; i < commands.size(); i++)
	{
		if (commands[i]->name == cmd) return i;
	}

	return -1;
}

void Console::parseToWords(const std::string& str, std::vector<std::string>& words)
{
	std::string word = "";
	int count = str.size();
	bool quote = false;
	for (int i = 0; i < count; i++)
	{
		char c = str[i];
		bool add = false;

		if (i == count - 1)
		{
			add = true;
		}

		if (c == '"')
		{
			quote = !quote;
		}

		if (c == ' ' && !quote)
		{
			add = true;
		}
		else if(c != '"')
		{
			word += c;
		}

		if (add)
		{
			words.push_back(word);
			word = "";
		}
	}
}

void Console::execute(const std::string& cmd, std::ostream& output)
{
	std::vector<std::string> words;

	parseToWords(cmd, words);

	//std::cout << "words: " << words.size() << std::endl;

	executedCommandCount = 0;

	bool blocked = false;

	std::vector<Parameter> parameters;
	int command = -1;
	for (int i = 0; i < words.size(); i++)
	{
		const std::string& word = words[i];
		int cmdIdx = findCommand(word);

		if (cmdIdx != -1 && blocked)
		{
			cmdIdx = -1;
			blocked = false;
		}

		//Add parameter if not command
		if (cmdIdx == -1)
		{
			parameters.push_back(Parameter(word));
		}

		//If command found or last word
		if (cmdIdx != -1 || i == words.size() - 1)
		{
			//Execute current command with the found parameters
			if (command != -1)
			{
				commands[command]->execute(parameters, output, this);
			}

			//Reset
			parameters.clear();
			command = cmdIdx;
			if(command != -1) blocked = commands[command]->blockNext;
		}
	}

	if (command != -1)
	{
		//Execute last command
		commands[command]->execute(parameters, output, this);
	}

	if (executedCommandCount == 0)
	{
		output << "No command was identified, use help to see available commands" << std::endl;
	}
}

void Console::runConsole(std::istream& input, std::ostream& output)
{
	std::cout << startMessage;

	std::string cmd = "";
	std::cout << cmdSymbol;
	while (std::getline(input, cmd))
	{
		execute(cmd, std::cout);
		output << cmdSymbol;

		if (quit) break;
	}
}
#include <iostream>

#include "Command.h"
#include "BasicCommands.h"
#include "FileCheck.h"

int main()
{
    Console console = Console({new HelloCommand(), new HelpCommand(), new QuitCommand(), new FilesCommand(), new CheckCommand(), new CountCommand(), new PatternCommand()},
       "[>] ", "Welcome to file checker, you can use the command help to get started\n");

    console.runConsole(std::cin, std::cout);
}
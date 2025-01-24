#include <iostream>

#include "Command.h"
#include "BasicCommands.h"
#include "FileCheck.h"

int main()
{
    Console console = Console({new HelloCommand(), new HelpCommand(), new QuitCommand(), new FilesCommand(), new CheckCommand(), new CountCommand()});

    std::string symbol = "[>] ";

    std::string cmd = "";
    std::cout << symbol;
    while (std::getline(std::cin, cmd))
    {        
        console.execute(cmd, std::cout);
        std::cout << symbol;

        if (console.quit) break;
    }
}
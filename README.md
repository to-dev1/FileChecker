# FileChecker

A C++ console program intended to quickly check the contents of text files in a targeted directory. Can be used for example to check that a backup contains everything intended.

# Guide

Once started, the user can write commands into the console. The command help can be used to see all other available commands and their explanations.

# Structure

New commands can be easily added by inheriting the Command class and adding the new command to the construction of a Console.

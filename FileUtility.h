#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

//Recursively get all files
void getAllFiles(const std::filesystem::path& path, std::vector<std::filesystem::directory_entry>& items, const bool recursive);

//Get all lines from the specified file
void getFileLines(const std::filesystem::path& path, std::vector<std::string>& lines);
#pragma once

#include <string>
#include <vector>

int Contains(std::string& base, char c);
bool StartsWith(std::string& base, const std::string& start);
std::vector<std::string> Split(std::string& toSplit, char c);

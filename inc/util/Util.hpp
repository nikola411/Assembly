#pragma once

#include <string>
#include <vector>

/// String utility
int Contains(std::string& base, char c);
bool StartsWith(std::string& base, const std::string& start);
std::vector<std::string> Split(std::string& toSplit, char c);
std::string Strip(std::string toStrip, char c = ' ');
std::string IntToHex(const uint32_t val);

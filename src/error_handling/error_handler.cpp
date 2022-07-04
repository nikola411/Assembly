#include "error_handler.hpp"

#include <iostream>

ErrorHandler::ErrorHandler(std::string line, std::string file)
{
    this->is_msg = false;
    this->line = line;
    this->file = file;
}

ErrorHandler::ErrorHandler(std::string message)
{
    this->is_msg = true;
    this->message = message;
}

void ErrorHandler::handle()
{
    if (is_msg)
    {
        std::cout << message << "\n";
    }
    else
    {
        std::cout << "Error at line: " << this->line << " in file: " << this->file << "\n";
    }

    exit(-1);
}

ErrorHandler::~ErrorHandler()
{

}
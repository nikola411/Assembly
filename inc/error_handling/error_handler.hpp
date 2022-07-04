#ifndef ERROR_HANDLER_HPP
#define ERRPR_HANDLER_HPP

#include <string>

class ErrorHandler
{
public:
    ErrorHandler(std::string line, std::string file);
    ErrorHandler(std::string);
    void handle();
    ~ErrorHandler();
private:
    std::string line;
    std::string file;
    std::string message;

    bool is_msg;
};


#endif
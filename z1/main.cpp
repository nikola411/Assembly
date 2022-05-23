#include <iostream>
#include <string>

#include "driver.hpp"

using std::string;


int main(int argc, char* argv[])
{
    // PARSE ARGUMENTS
    std::cout << "Parsing arguments.\n";
    if (argc != 4)
    {
        std::cout << "FATAL! Wrong number of arguments! \n";
        exit(-1);
    }

    string flag = string(argv[1]);
    string output_file = string(argv[2]);
    string input_file = string(argv[3]);
    
    if (flag != "-o")
    {
        std::cout << "FATAL! No output flag specified! \n";
    }

    std::cout << "Starting to read the input file.\n";
    Driver driver;
    driver.parse(input_file);

    // MAKE INTERPRETER CLASS


    // START PARSING


    // CREATE SYMBOL TABLE

    
    // ASSEMBLER FIRST PASS
    

    // PRINT SYMBOL TALBE AND SECTIONS
    return 0;
}
#include <iostream>
#include <string>

#include "driver.hpp"
#include "assembly.hpp"

using std::string;


int main(int argc, char* argv[])
{
    // PARSE ARGUMENTS
    std::cout << "Parsing arguments.\n";
    if (argc > 6 || argc < 4)
    {
        std::cout << "FATAL! Wrong number of arguments! \n";
        exit(-1);
    }

    string flag = string(argv[1]);
    string output_file = string(argv[2]);
    string input_file = string(argv[3]);

    bool parsing = false, scanning = false;
    if (argc > 4)
    {
        parsing = bool(argv[4]);
        scanning = bool(argv[5]);
    }

    std::cout << "Parsing: " << parsing << " Scanning: " << scanning << std::endl;
    
    if (flag != "-o")
    {
        std::cout << "FATAL! No output flag specified! \n";
    }

    std::cout << "Starting to read the input file.\n";
    Driver driver(parsing, scanning);
    driver.parse(input_file);

    Assembly::print();

    Assembly::clean();
    // MAKE INTERPRETER CLASS


    // START PARSING


    // CREATE SYMBOL TABLE

    
    // ASSEMBLER FIRST PASS
    

    // PRINT SYMBOL TALBE AND SECTIONS
    return 0;
}
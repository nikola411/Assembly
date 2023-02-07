#include "linker.hpp"
#include "error_handler.hpp"
#include "relocation_table.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc <= 2)
    {
        std::cout << "ERROR! Not enought arguments provided! \n";
    }

    std::string output_file;
    std::vector<std::string> input_files;
    bool is_hex = false;
    bool read_input = false;
    bool read_output = false;

    for (int i = 1; i < argc; i++)
    {
        std::string current = std::string(argv[i]);
        if (current == "-o")
        {
            read_output = true;
        }
        else if (read_output)
        {
            std::cout << argv[i] << " ";
            output_file = argv[i];
            read_output = false;
        }
        else if (current == "-hex")
        {
            is_hex = true;
        }
        else
        {
            std::cout << current << " ";
            input_files.emplace_back(current);
        }
    }
    
    try
    {
        Linker lnk(input_files, output_file, is_hex);
        lnk.link();

        if (is_hex)
        {
            lnk.generate_hex();
        }
        else
        {

        }
        
    }
    catch(ErrorHandler e)
    {
        e.handle();
    }

    
    
    return 0;
}
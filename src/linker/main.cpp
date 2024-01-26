#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <set>

#include "Linker.hpp"

#include "Util.hpp"
#include "AssemblyUtil.hpp"
#include "LinkerUtil.hpp"

using namespace LinkerUtil;

int main(int argc, char* argv[])
{
    std::vector<std::string> argVector = {};
    for (auto i = 1; i < argc; ++i)
    {
        argVector.push_back(std::string(argv[i]));
    }

    LinkerArguments arguments;
    try
    {
        arguments.ParseArguments(argVector);
        Linker linker(arguments);
        linker.StartLinking();
        linker.WriteOutput();
    }
    catch(LinkerException& e)
    {
        std::cerr << "\033[31mERROR: " << e.what() << '\n';
    }
    catch(std::exception& e)
    {
        std::cerr << "\033[31m ERROR: Caught unhandled exception! \n";
    }
    
    return 0;
}
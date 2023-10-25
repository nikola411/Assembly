#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <map>
#include <set>

#include "linker.hpp"
#include "util.hpp"

/*
    linker [opcije] <naziv_ulazne_datoteke>...
    [opcije]:
        * -hex
        * -o <naziv_izlazne_datoteke>
        * -place=<ime_sekcije>@<adresa>
        * -relocatable
*/

using namespace LinkerUtil;

void ParseArguments(std::vector<std::string>& argVector, LinkerArguments& args)
{
    ArgumentParsingState next = DO_NOTHING;

    for (auto arg: argVector)
    {
        if (arg == LINKER_FLAG)
            continue;

        if (arg == HEX_FLAG)
        {
            if (args.relocatable) throw LinkerException("Cannot have -relocatable and -hex together");
            args.hex = true;
            continue;
        }

        if (arg == RELOCATABLE_FLAG)
        {
            if (args.hex) throw LinkerException("Cannot have -relocatable and -hex together");
            args.relocatable = true;
            continue;
        }
        
        if (arg == OUT_FLAG)
        {
            next = READ_OUTPUT;
            continue;
        }

        if (StartsWith(arg, PLACE_FLAG))
        {
            args.place.push_back(arg.substr(PLACE_FLAG.size(), arg.size()));
            continue;
        }

        if (next == READ_OUTPUT)
        {
            args.outFile = arg;
            next = DO_NOTHING;
            continue;
        }

        args.inFiles.push_back(arg);
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::string> argVector = {};
    for (auto i = 0; i < argc; ++i)
    {
        argVector.push_back(std::string(argv[i]));
    }

    LinkerArguments argStruct;
    try
    {
        ParseArguments(argVector, argStruct);
        Linker linker(argStruct);
        linker.StartLinking();
        linker.WriteOutput();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
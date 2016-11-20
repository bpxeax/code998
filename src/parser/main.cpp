#include "parser.h"
#include <iostream>

using namespace CoolMonkey;
int main(int argc, char* argv[])
{
    MetaDataParserOptions parser_options;

#ifdef DEBUG
    parser_options.m_display_debug_info = true;
#else
    parser_options.m_display_debug_info = false;
#endif

    if (argc < 4)
    {
        std::cerr << "lack of parser argument!" << std::endl;
    }

    if (std::string(argv[1]) == "true")
    {
        parser_options.m_display_debug_info = true;
    }
    else if (std::string(argv[1]) == "false")
    {
        parser_options.m_display_debug_info = false;
    }
    else
    {
        
    }

    parser_options.m_source_file_dir = std::string(argv[2]);
    parser_options.m_dest_file_dir = std::string(argv[3]);

    for (int index = 4; index < argc; ++index)
    {
        parser_options.m_arguments.emplace_back(argv[index]);
    }

    MetaDataParser parser(parser_options);
    parser.Parse();

    return 0;
}
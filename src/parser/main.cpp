#include "parser.h"

using namespace CoolMonkey;
int main(int argc, char* argv[])
{
    MetaDataParserOptions parser_options;
    parser_options.m_display_debug_info = true;
    parser_options.m_source_file_name = "D:/code998/src/test/parser/enum.h";
    parser_options.m_arguments.emplace_back("-x");
    parser_options.m_arguments.emplace_back("c++");
    parser_options.m_arguments.emplace_back("-std=c++1y");

    MetaDataParser parser(parser_options);
    parser.Parse();

    return 0;
}
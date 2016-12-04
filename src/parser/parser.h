#ifndef __COOLMONKEY_PARSER_H__
#define __COOLMONKEY_PARSER_H__

#include <string>
#include <clang-c/Index.h>
#include "parser_options.h"

namespace CoolMonkey
{
    class Class;
    class External;
    class Global;
    class Function;
    class Enum;

    class MetaDataParser
    {
    public:
        MetaDataParser(const MetaDataParserOptions &options);
        ~MetaDataParser(void);

        bool Parse(void);

    private:
        static void getHeaderFilesInDirectory(const std::string& path, std::vector<std::string>& out_header_files);
        static bool parseSingleFile(const std::string& file_path, bool is_debug, std::vector<const char*>& arguments);

    private:
        MetaDataParserOptions m_parse_options;
    };
}

#endif
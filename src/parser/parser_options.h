#ifndef __COOLMONKEY_PARSER_OPTIONS_H__
#define __COOLMONKEY_PARSER_OPTIONS_H__

#include <vector>
#include <string>

namespace CoolMonkey
{
    class MetaDataParserOptions
    {
    public:
        bool m_force_rebuild{ false };
        bool m_display_debug_info{ false };

        std::string m_source_file_name;
        std::vector<std::string> m_arguments;
    };
}

#endif

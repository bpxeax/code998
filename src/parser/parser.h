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

        void Parse(void);

    private:
        /*void buildClasses(
            const Cursor &cursor,
            Namespace &currentNamespace
        );

        void buildGlobals(
            const Cursor &cursor,
            Namespace &currentNamespace
        );

        void buildGlobalFunctions(
            const Cursor &cursor,
            Namespace &currentNamespace
        );

        void buildEnums(
            const Cursor &cursor,
            Namespace &currentNamespace
        );*/

    private:
        CXIndex m_index {nullptr};
        CXTranslationUnit m_translationUnit {nullptr};
        MetaDataParserOptions m_parse_options;
    };
}

#endif
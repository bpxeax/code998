#include "parser.h"
#include <fstream>
#include <iostream>


namespace CoolMonkey
{
#define RECURSE_NAMESPACES(kind, cursor, method, ns) \
    if (kind == CXCursor_Namespace)                  \
    {                                                \
        auto displayName = cursor.GetDisplayName( ); \
        if (!displayName.empty( ))                   \
        {                                            \
            ns.emplace_back( displayName );          \
            method( cursor, ns );                    \
            ns.pop_back( );                          \
        }                                            \
    }                                                \

#define TRY_ADD_LANGUAGE_TYPE(handle, container)                \
    if (handle->ShouldCompile( ))                               \
    {                                                           \
        auto file = handle->GetSourceFile( );                   \
        m_moduleFiles[ file ].container.emplace_back( handle ); \
    }                                                           \

#define COMPILE_TYPE_TEMPLATES(data, listName, container)   \
    {                                                       \
        TemplateData typeData { TemplateData::Type::List }; \
        for (auto &handle : container)                      \
            typeData << handle->CompileTemplate( this );    \
        data[ listName ] = typeData;                        \
    }

    MetaDataParser::MetaDataParser(const MetaDataParserOptions &options)
        : m_parse_options(options)
        , m_index(nullptr)
        , m_translationUnit(nullptr)
    {

    }

    MetaDataParser::~MetaDataParser(void)
    {
        if (m_translationUnit)
        {
            clang_disposeTranslationUnit(m_translationUnit);
        }

        if (m_index)
        {
            clang_disposeIndex(m_index);
        }
    }

    void MetaDataParser::Parse(void)
    {
        m_index = clang_createIndex(true, m_parse_options.m_display_debug_info);

        std::vector<const char*> arguments;

        if (m_parse_options.m_display_debug_info)
        {
            std::cout << "---------- parse arguments ----------" << std::endl;
        }

        for (const auto &argument : m_parse_options.m_arguments)
        {
            if (m_parse_options.m_display_debug_info)
            {
                std::cout << argument << std::endl;
            }
            arguments.emplace_back(argument.c_str());
        }

        if (m_parse_options.m_display_debug_info)
        {
            std::cout << "-------------------------------------" << std::endl;
        }

        m_translationUnit = clang_createTranslationUnitFromSourceFile(
            m_index,
            m_parse_options.m_source_file_name.c_str(),
            static_cast<int>(arguments.size()),
            arguments.data(),
            0,
            nullptr
        );

        CXCursor cursor = clang_getTranslationUnitCursor(m_translationUnit);

        auto visitor = [](CXCursor child_cursor, CXCursor parent_cursor, CXClientData data) -> CXChildVisitResult
        {
            CXString kind_string = clang_getCursorDisplayName(child_cursor);
            std::cout << clang_getCString(kind_string) << std::endl;
            clang_disposeString(kind_string);

            return CXChildVisitResult::CXChildVisit_Continue;
        };

        clang_visitChildren(cursor, visitor, nullptr);
    }
}

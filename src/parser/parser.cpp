#include "parser.h"
#include <fstream>
#include <iostream>
#include <list>


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
        m_index = clang_createIndex(1, static_cast<int>(m_parse_options.m_display_debug_info));

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
            if (clang_getCursorKind(child_cursor) == CXCursorKind::CXCursor_MacroDefinition)
            {
                return CXChildVisitResult::CXChildVisit_Continue;
            }

            std::list<CXCursor>* child_cursors = static_cast<std::list<CXCursor>*>(data);

            CXString display_name_string = clang_getCursorDisplayName(child_cursor);
            CXString kind_string = clang_getCursorKindSpelling(clang_getCursorKind(child_cursor));
            CXString spelling_string = clang_getCursorSpelling(child_cursor);
            CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(child_cursor);

            std::cout << "display: " << clang_getCString(display_name_string) << std::endl;
            std::cout << "kind: " << clang_getCString(kind_string) << std::endl;
            std::cout << "spelling: " << clang_getCString(spelling_string) << std::endl;
            std::string access_string = "invalid";
            switch (access)
            {
            case CX_CXXAccessSpecifier::CX_CXXPrivate:
                access_string = "private";
                break;
            case CX_CXXAccessSpecifier::CX_CXXProtected:
                access_string = "protected";
                break;
            case CX_CXXAccessSpecifier::CX_CXXPublic:
                access_string = "public";
                break;
            }

            std::cout << "access: " << access_string << std::endl;

            if (clang_getCursorKind(child_cursor) == CXCursorKind::CXCursor_AnnotateAttr)
            {
                CXString parent_string = clang_getCursorDisplayName(parent_cursor);
                std::cout << "parent: " << clang_getCString(parent_string) << std::endl;
                clang_disposeString(parent_string);
            }

            clang_disposeString(display_name_string);
            clang_disposeString(kind_string);
            clang_disposeString(spelling_string);

            if (child_cursors)
            {
                child_cursors->push_back(child_cursor);
            }

            return CXChildVisitResult::CXChildVisit_Continue;
        };

        std::list<CXCursor> cursors;
        clang_visitChildren(cursor, visitor, &cursors);

        while(cursors.empty() == false)
        {
            clang_visitChildren(cursors.front(), visitor, &cursors);
            cursors.pop_front();
        }
    }
}

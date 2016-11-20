#include "parser.h"
#include "boost/filesystem.hpp"
#include <iostream>
#include <list>


namespace CoolMonkey
{
    MetaDataParser::MetaDataParser(const MetaDataParserOptions &options)
        : m_parse_options(options)
    {

    }

    MetaDataParser::~MetaDataParser(void)
    {

    }

    void MetaDataParser::Parse(void)
    {
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

        std::vector<std::string> all_header_files;
        getHeaderFilesInDirectory(m_parse_options.m_source_file_dir, all_header_files);

        for (const auto& header_file : all_header_files)
        {
            parseSingleFile(header_file, m_parse_options.m_display_debug_info, arguments);
        }
    }

    void MetaDataParser::getHeaderFilesInDirectory(const std::string& path, std::vector<std::string>& out_header_files)
    {
        boost::filesystem::recursive_directory_iterator iter_end;
        boost::filesystem::recursive_directory_iterator src_file_iter(path);
        for (; src_file_iter != iter_end; ++src_file_iter)
        {
            auto current_path = src_file_iter->path();
            if (boost::filesystem::is_directory(current_path))
            {
                getHeaderFilesInDirectory(current_path.string(), out_header_files);
            }
            else
            {
                const std::string file_extension = current_path.extension().string();
                if (file_extension == ".h" ||
                    file_extension == ".hpp")
                {
                    out_header_files.emplace_back(current_path.string());
                }
            }
        }
    }

    void MetaDataParser::parseSingleFile(const std::string& file_path, bool is_debug, std::vector<const char*>& arguments)
    {
        CXIndex cursor_index = clang_createIndex(1, static_cast<int>(is_debug));

        CXTranslationUnit translation_unit = clang_createTranslationUnitFromSourceFile(
            cursor_index,
            file_path.c_str(),
            static_cast<int>(arguments.size()),
            arguments.data(),
            0,
            nullptr
        );

        CXCursor cursor = clang_getTranslationUnitCursor(translation_unit);

        auto visitor = [](CXCursor child_cursor, CXCursor parent_cursor, CXClientData data) -> CXChildVisitResult
        {
            if(clang_Location_isInSystemHeader(clang_getCursorLocation(child_cursor)) != 0)
            {
                return CXChildVisitResult::CXChildVisit_Continue;
            }

            CXCursorKind current_cursor_kind = clang_getCursorKind(child_cursor);

            /*if (current_cursor_kind == CXCursorKind::CXCursor_LastPreprocessing)
            {
                return CXChildVisitResult::CXChildVisit_Break;
            }*/

            std::list<CXCursor>* child_cursors = static_cast<std::list<CXCursor>*>(data);

            if (current_cursor_kind == CXCursorKind::CXCursor_AnnotateAttr)
            {
                CXString display_name_string = clang_getCursorDisplayName(child_cursor);
                CXString kind_string = clang_getCursorKindSpelling(clang_getCursorKind(child_cursor));
                CXString spelling_string = clang_getCursorSpelling(child_cursor);
                CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(child_cursor);

                CXSourceLocation cursor_location = clang_getCursorLocation(child_cursor);
                CXFile file;
                unsigned int  line, column, offset;
                clang_getFileLocation(cursor_location, &file, &line, &column, &offset);
                CXString file_name_string = clang_getFileName(file);
                std::cout << "parsing file: " << clang_getCString(file_name_string) << std::endl;
                clang_disposeString(file_name_string);

                std::cout << "----------------------------------------------------" << std::endl;
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
                default:
                    break;
                }

                std::cout << "access: " << access_string << std::endl;

                CXString parent_string = clang_getCursorDisplayName(parent_cursor);
                std::cout << "parent: " << clang_getCString(parent_string) << std::endl;
                clang_disposeString(parent_string);

                std::cout << "----------------------------------------------------" << std::endl;

                clang_disposeString(display_name_string);
                clang_disposeString(kind_string);
                clang_disposeString(spelling_string);
            }

            if (child_cursors)
            {
                child_cursors->push_back(child_cursor);
            }

            return CXChildVisitResult::CXChildVisit_Continue;
        };

        std::list<CXCursor> cursors;
        clang_visitChildren(cursor, visitor, &cursors);

        while (cursors.empty() == false)
        {
            clang_visitChildren(cursors.front(), visitor, &cursors);
            cursors.pop_front();
        }

        clang_disposeTranslationUnit(translation_unit);
        clang_disposeIndex(cursor_index);
    }

}

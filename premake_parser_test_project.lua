function genParserTestProject()
    project "ParserTest" 
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "ConsoleApp"
        language "C++"
        targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")
        
        includedirs
        {
            path.join(GLOBAL.SRC_DIR);
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "test/parser", "**.h"),
            path.join(GLOBAL.SRC_DIR, "test/parser", "**.cpp")
        }

        -- local all_header_files = os.matchfiles(path.join(GLOBAL.SRC_DIR, "core/**.h"));
        -- local header_file_list_string = ""
        -- table.foreach(all_header_files, 
        -- function(_, v)
        --     header_file_list_string = header_file_list_string..v..string.format( "\n");
        -- end);
        -- header_file_list_string = '"'..header_file_list_string..'"';
        -- print(header_file_list_string);
        -- local command = "{ECHO} "..header_file_list_string;
        -- prebuildcommands (command);
end
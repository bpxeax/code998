function genCodeGenProject(prj_name)
    project(prj_name)
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "Utility"

        files
        {
            path.join(GLOBAL.SRC_DIR, "**.h"),
            path.join(GLOBAL.SRC_DIR, "**.hpp")
        }

        vpaths
        {
            ["parse_headers"] = {"**.h", "**.hpp"}
        }

        local common_parse_arguments = "-x c++ -std=c++1y -fsyntax-only -DCOOL_MONKEY_METADATA_PARSER"
        common_parse_arguments = common_parse_arguments.." -I"..GLOBAL.SRC_DIR.." -I"..GLOBAL.THIRD_SRC_DIR.." -I"..path.join(GLOBAL.THIRD_SRC_DIR, "asio").." -I"..path.join(GLOBAL.THIRD_SRC_DIR, "libuv/include").." -I"..path.join("$(LLVM_ROOT)", "include").." -I"..path.join("$(BOOST_ROOT)")
        local windows_parse_arguments = common_parse_arguments.." -fno-ms-compatibility"

        prebuildmessage("MetaData parse");

        print(windows_parse_arguments);
        filter {"system:windows"}
            prebuildcommands
            {
                GLOBAL.GENERATORS_DIR.."/MetaDataParser".." ".."none".." "..GLOBAL.SRC_DIR.." "..GLOBAL.GEN_SRC_DIR.." "..windows_parse_arguments
            }
        filter {"system:not windows"}
            prebuildcommands
            {
                GLOBAL.GENERATORS_DIR.."/MetaDataParser".." ".."none".." "..GLOBAL.SRC_DIR.." "..GLOBAL.GEN_SRC_DIR.." "..common_parse_arguments
            }
        filter {}
end
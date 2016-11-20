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

        local parse_arguments = "-x c++ -std=c++1y -DCOOL_MONKEY_METADATA_PARSER";

        prebuildmessage("MetaData parse");
        prebuildcommands
        {
            GLOBAL.GENERATORS_DIR.."/MetaDataParser".." ".."none".." "..GLOBAL.SRC_DIR.." "..GLOBAL.GEN_SRC_DIR.." "..parse_arguments
        }
end
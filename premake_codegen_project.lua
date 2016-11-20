function genCodeGenProject(prj_name)
    project(prj_name)
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "Utility"

        prebuildmessage("MetaData parse");
        prebuildcommands
        {
            GLOBAL.GENERATORS_DIR.."/MetaDataParser".." "..GLOBAL.SRC_DIR.." "..GLOBAL.GEN_SRC_DIR
        }
end
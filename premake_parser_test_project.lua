function genParserTestProject(prj_name)
    project(prj_name)
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
end
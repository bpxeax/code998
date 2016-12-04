local prj_name="ParserTest"

function genParserTestProject()
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
            path.join(GLOBAL.SRC_DIR, "test/parser", "**.hpp"),
            path.join(GLOBAL.SRC_DIR, "test/parser", "**.cpp")
        }

    return prj_name
end
function genServerProject(prj_name)
    project(prj_name)
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "ConsoleApp"
        language "C++"
        targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")

        includedirs
        {
            path.join(GLOBAL.THIRD_SRC_DIR, "asio")
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "server", "**.h"),
            path.join(GLOBAL.SRC_DIR, "server", "**.hpp"),
            path.join(GLOBAL.SRC_DIR, "server", "**.cpp")
        }

        defines
        {
            "ASIO_STANDALONE"
        }

        filter "system:windows" 
            defines 
            {
                "_WIN32",
                "_WIN32_WINNT=0x0501"
            } 

            links 
            {
                
            } 

        filter "system:not windows" 
            links 
            {
                --"pthread"
            }
end
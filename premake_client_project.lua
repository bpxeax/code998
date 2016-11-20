function genClientProject(prj_name)
    project(prj_name)
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "ConsoleApp"
        language "C++"
        targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")

        includedirs
        {
            path.join(GLOBAL.SRC_DIR, "3rd/asio")
        }

        defines
        {
            "ASIO_STANDALONE"
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "client", "**.h"),
            path.join(GLOBAL.SRC_DIR, "client", "**.cpp")
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
local prj_name = "client"

function genClientProject()
    project(prj_name)
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "ConsoleApp"
        language "C++"
        targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")

        includedirs
        {
            path.join(GLOBAL.THIRD_SRC_DIR, "asio")
        }

        defines
        {
            "ASIO_STANDALONE"
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "client", "**.h"),
            path.join(GLOBAL.SRC_DIR, "client", "**.hpp"),
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
                "core"
            } 

        filter "system:not windows" 
            links 
            {
                "core",
                "pthread"
            }

    return prj_name
end
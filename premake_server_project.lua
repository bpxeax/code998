function genServerProject()
    project "server"
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "ConsoleApp"
        language "C++"
        targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")

        includedirs
        {
            path.join(GLOBAL.SRC_DIR, "3rd/libuv/include"),
            path.join(GLOBAL.SRC_DIR, "3rd/asio")
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "server", "**.h"),
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
                "_WIN32_WINNT=0x0600",
                "_GNU_SOURCE"
            } 

            links 
            {
                "ws2_32", 
                "Iphlpapi", 
                "userenv", 
                "psapi", 
                "MSVCRTD",
                "libuv_".."%{cfg.buildcfg}_".."%{cfg.platform}"
            } 

        filter "system:not windows" 
            links 
            {
                "uv", 
                "pthread"
            }
end
local prj_name="S5ProxyTest"

function genS5ProxyTestProject()
    project(prj_name)
        targetname "S5Proxy"
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "ConsoleApp"
        language "c"
        targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")

        includedirs
        {
            path.join(GLOBAL.THIRD_SRC_DIR, "libuv/include")
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "test", "**.h"),
            path.join(GLOBAL.SRC_DIR, "test", "**.c")
        }

        removefiles
        {
            path.join(GLOBAL.SRC_DIR, "test", "getopt.c")
        }

        filter "system:windows" 
            defines 
            {
                "HAVE_UNISTD_H=0", 
                "_WIN32",
                "_WIN32_WINNT=0x0600",
                "_GNU_SOURCE"
            } 

            files 
            {
                path.join(GLOBAL.SRC_DIR, "test", "getopt.c")
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
            defines 
            {
                "HAVE_UNISTD_H=1"
            } 

            links 
            {
                "uv", 
                "pthread"
            }

    return prj_name
end
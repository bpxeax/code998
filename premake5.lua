dofile("premake_common.lua");

solution "Jungle"
    configurations {"debug", "release"}
    platforms {"x86", "x64"}
    location (GLOBAL.BUILD_DIR)
    libdirs {path.join(GLOBAL.PACKAGE_DIR, "/**")}

    filter {"configurations:debug"}
        defines "DEBUG"
        symbols "on"
        
    filter {"configurations:release"}
        defines "NDEBUG"
        optimize "on"

    filter {"platforms: x86"}
        architecture "x86"

    filter {"platforms: x64"}
        architecture "x86_64"

    filter {"language:c++", "action:gmake"}
        buildoptions "-std=c++11"

    filter {}

project "core" 
    location (GLOBAL.BUILD_DIR.."/%{prj.name}")
    kind "StaticLib"
    language "C++"
    targetdir (GLOBAL.LIB_OUT_DIR.."/%{prj.name}")
    includedirs 
    {
        "src/3rd/libuv/include"
    }

    files
    {
        "src/core/**.h",
        "src/core/**.cpp"
    }

project "server"
    location (GLOBAL.BUILD_DIR.."/%{prj.name}")
    kind "ConsoleApp"
    language "C++"
    targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")

    includedirs
    {
        "src/3rd/libuv/include"
    }

    files
    {
        "src/server/**.h",
        "src/server/**.cpp"
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

project "client"
    location (GLOBAL.BUILD_DIR.."/%{prj.name}")
    kind "ConsoleApp"
    language "C++"
    targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")

    includedirs
    {
        "src/3rd/libuv/include"
    }

    files
    {
        "src/client/**.h",
        "src/client/**.cpp"
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



project "S5ProxyTest"
    targetname "S5Proxy"
    location (GLOBAL.BUILD_DIR.."/%{prj.name}")
    kind "ConsoleApp"
    language "c"
    targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")

    includedirs
    {
        "src/3rd/libuv/include"
    }

    files
    {
        "test/**.h", "test/**.c"
    }

    excludes
    {
        "test/getopt.c"
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
            "test/getopt.c"
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


if _ACTION == "clean" then
    os.rmdir "output" 
    os.rmdir "build" 
end
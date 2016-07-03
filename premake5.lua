solution("Server");
    configurations({"debug", "release"});
    location("build");
    libdirs({"package/**"});
    
project("S5ProxyTest");
    targetname("S5Proxy");
    location("build/S5ProxyTest");
    kind("ConsoleApp");
    language("c");
    targetdir("bin/%{cfg.buildcfg}");
    includedirs({"src/3rd/libuv/include"});
    files({"test/**.h", "test/**.c"});
    removefiles({"test/getopt.c"});
    
    filter("configurations:debug");
        defines("DEBUG");
        symbols("on");
        
    filter("configurations:release");
        defines("NDEBUG");
        optimize("on");

    filter("system:windows");
        defines({"HAVE_UNISTD_H=0", "_WIN32", "_WIN32_WINNT=0x0600", "_GNU_SOURCE"});
        files({"test/getopt.c"});
        links({"ws2_32", "Iphlpapi", "userenv", "psapi", "MSVCRTD"});

    filter("system:linux");
        defines("HAVE_UNISTD_H=1");
        links({"uv", "pthread"});

    filter({"system:windows", "configurations:debug"});
        links({"libuv_d_x86"});

    filter({"system:windows" ,"configurations:release"});
        links({"libuv_r_x86"});


if _ACTION == "clean" then
    os.rmdir("bin");
    os.rmdir("build");
end
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

    filter("system:windows");
        defines("HAVE_UNISTD_H=0");
        defines("_WIN32")
        files({"test/getopt.c"});

    filter("system:linux");
        defines("HAVE_UNISTD_H=1");

    filter("configurations:debug");
        defines("DEBUG");
        symbols("on");
        links({"libuv_d_x86"});

    filter("configurations:release");
        defines("NDEBUG");
        optimize("on");
        links({"libuv_r_x86"});


if _ACTION == "clean" then
    os.rmdir("bin");
    os.rmdir("build");
end
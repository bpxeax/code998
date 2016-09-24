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

    dofile("premake_core_project.lua")
    
    dofile("premake_client_project.lua")
    dofile("premake_server_project.lua")
    dofile("premake_s5proxy_project.lua")

    genCoreProject()
    genClientProject()
    genServerProject()
    genS5ProxyTestProject()

if _ACTION == "clean" then
    os.rmdir (GLOBAL.OUT_ROOT_DIR)
    os.rmdir (GLOBAL.BUILD_ROOT_DIR)
end
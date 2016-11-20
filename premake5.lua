dofile("premake_common.lua");

solution "Tools"
    configurations {"debug", "release"}
    platforms {"x86", "x64"}
    location (GLOBAL.BUILD_DIR)
    libdirs {path.join(GLOBAL.PACKAGE_DIR, "**")}

    filter {"configurations:debug"}
        defines "DEBUG"
        symbols "On"
        optimize "off"
        
    filter {"configurations:release"}
        defines "NDEBUG"
        optimize "speed"

    filter {"platforms: x86"}
        architecture "x86"

    filter {"platforms: x64"}
        architecture "x86_64"

    filter {"language:c++"}
        flags "c++11"

    filter {}

    dofile("premake_parser_project.lua")

    ------ start generate projects ------

    genParserProject("parser")

    ------ end generate projects ------


solution "Jungle"
    configurations {"debug", "release"}
    platforms {"x86", "x64"}
    location (GLOBAL.BUILD_DIR)
    libdirs {path.join(GLOBAL.PACKAGE_DIR, "**")}

    filter {"configurations:debug"}
        defines "DEBUG"
        symbols "On"
        optimize "off"
        
    filter {"configurations:release"}
        defines "NDEBUG"
        optimize "speed"

    filter {"platforms: x86"}
        architecture "x86"

    filter {"platforms: x64"}
        architecture "x86_64"

    filter {"language:c++"}
        flags "c++11"

    filter {}

    dofile("premake_codegen_project.lua")

    dofile("premake_core_project.lua")
    dofile("premake_client_project.lua")
    dofile("premake_server_project.lua")

    dofile("premake_parser_test_project.lua");

    ------ start generate projects ------

    group "generators"
        genCodeGenProject("CodeGen")
    group "runtime"
        genCoreProject("core")
        genClientProject("client")
        genServerProject("server")
    group "tests"
        genParserTestProject("ParserTest")
    group ""

    project "*"

    ------ end generate projects -------

    ------ set project dependencies ------

    project "core"
        dependson {"CodeGen"}
    project "client"
        dependson {"CodeGen"}
    project "server"
        dependson {"CodeGen"}
    project "ParserTest"
        dependson {"CodeGen"}
    project "*"

    --------------------------------------

solution "*"

local clean = function()
    os.rmdir (GLOBAL.OUT_ROOT_DIR)
    os.rmdir (GLOBAL.BUILD_ROOT_DIR)
end
premake.addCleanAction(clean)
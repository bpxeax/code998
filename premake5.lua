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

    local parser_prj_name = genParserProject()

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
        local code_gen_prj_name = genCodeGenProject()
    group "runtime"
        local core_prj_name = genCoreProject()
        local client_prj_name = genClientProject()
        local server_prj_name = genServerProject()
    group "tests"
        local parser_test_prj_name = genParserTestProject()
    group ""

    project "*"

    ------ end generate projects -------

    ------ set project dependencies ------

    project(core_prj_name)
        dependson {code_gen_prj_name}
    project(client_prj_name)
        useCoreLib()
        dependson {code_gen_prj_name, core_prj_name}
    project(server_prj_name)
        useCoreLib()
        dependson {code_gen_prj_name, core_prj_name}
    project(parser_test_prj_name)
        dependson {code_gen_prj_name}
    project "*"

    --------------------------------------

solution "*"

local clean = function()
    os.rmdir (GLOBAL.OUT_ROOT_DIR)
    os.rmdir (GLOBAL.BUILD_ROOT_DIR)
end
premake.addCleanAction(clean)
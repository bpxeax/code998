local prj_name = "parser"

function genParserProject()
    project(prj_name) 
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "ConsoleApp"
        language "C++"
        targetname("MetaDataParser")
        targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")
        libdirs 
        {
            path.join(os.getenv("LLVM_ROOT"), "lib"),
            path.join(os.getenv("BOOST_ROOT"), "lib");
        }
        
        includedirs 
        {
            path.join(os.getenv("LLVM_ROOT"), "include");
            path.join(os.getenv("BOOST_ROOT"), "include");
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "core/compile/common_macros.h"),
            path.join(GLOBAL.SRC_DIR, "parser", "**.h"),
            path.join(GLOBAL.SRC_DIR, "parser", "**.hpp"),
            path.join(GLOBAL.SRC_DIR, "parser", "**.cpp")
        }

        links
        {
            "clang",
            "boost_filesystem",
            "boost_system"
        }

        postbuildcommands
        {
            "{MKDIR} "..GLOBAL.GENERATORS_DIR,
            "{COPY} %{cfg.linktarget.abspath} "..GLOBAL.GENERATORS_DIR
        }

    return prj_name
end
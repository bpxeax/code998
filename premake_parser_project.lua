function genParserProject(prj_name)
    project(prj_name) 
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "ConsoleApp"
        language "C++"
        targetname("MetaDataParser")
        targetdir (GLOBAL.BIN_OUT_DIR.."/%{prj.name}")
        libdirs {path.join("$(LLVM_ROOT)", "lib")}
        
        includedirs 
        {
            path.join("$(LLVM_ROOT)", "include");
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "parser", "**.h"),
            path.join(GLOBAL.SRC_DIR, "parser", "**.cpp")
        }

        links
        {
            "libclang"
        }

        postbuildcommands
        {
            "{COPY} %{cfg.linktarget.abspath} "..GLOBAL.GENERATORS_DIR
        }
end
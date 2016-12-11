local prj_name = "core"

function genCoreProject()
    project(prj_name)
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "StaticLib"
        language "C++"
        targetdir (GLOBAL.LIB_OUT_DIR.."/%{prj.name}")
        
        libdirs 
        {
            path.join("$(LUAJIT_ROOT)", "src")
        }

        includedirs 
        {
            path.join(GLOBAL.THIRD_SRC_DIR, "asio"),
            path.join("$(LUAJIT_ROOT)", "src")
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "core", "**.h"),
            path.join(GLOBAL.SRC_DIR, "core", "**.hpp"),
            path.join(GLOBAL.SRC_DIR, "core", "**.cpp")
        }

        defines
        {
            "ASIO_STANDALONE"
        }

        links
        {
            "lua51"
        }

        filter "system:windows" 
            defines 
            {
                "_WIN32",
                "_WIN32_WINNT=0x0501"
            } 

    return prj_name
end

function useCoreLib()
    libdirs 
    {
        path.join("$(LUAJIT_ROOT)", "src")
    }

    includedirs 
    {
        path.join("$(LUAJIT_ROOT)", "src")
    }

    includedirs{path.join(GLOBAL.SRC_DIR, "core")}
    links 
    {
        prj_name,
        "lua51"
    }
end
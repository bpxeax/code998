local prj_name = "core"

function genCoreProject()
    project(prj_name)
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "StaticLib"
        language "C++"
        targetdir (GLOBAL.LIB_OUT_DIR.."/%{prj.name}")
        
        libdirs 
        {
            path.join(os.getenv("LUAJIT_ROOT"), "lib"),
            path.join(os.getenv("BOOST_ROOT"), "lib")
        }

        includedirs 
        {
            path.join(GLOBAL.THIRD_SRC_DIR, "asio"),
            path.join(os.getenv("LUAJIT_ROOT"), "include/luajit-2.0"),
            path.join(os.getenv("BOOST_ROOT"), "include")
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
            "luajit-5.1"
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
        path.join(os.getenv("LUAJIT_ROOT"), "lib")
    }

    includedirs 
    {
        path.join(os.getenv("LUAJIT_ROOT"), "include/luajit-2.0")
    }

    includedirs{path.join(GLOBAL.SRC_DIR, "core")}
    links 
    {
        prj_name,
        "luajit-5.1"
    }

    filter "system:not windows"
        links
        {
            "dl"
        }
end
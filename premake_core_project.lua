function genCoreProject(prj_name)
    project(prj_name)
        location (GLOBAL.BUILD_DIR.."/%{prj.name}")
        kind "StaticLib"
        language "C++"
        targetdir (GLOBAL.LIB_OUT_DIR.."/%{prj.name}")
        
        includedirs 
        {
            path.join(GLOBAL.SRC_DIR, "3rd/asio/")
        }

        files
        {
            path.join(GLOBAL.SRC_DIR, "core", "**.h"),
            path.join(GLOBAL.SRC_DIR, "core", "**.cpp")
        }

        defines
        {
            "ASIO_STANDALONE"
        }

        filter "system:windows" 
            defines 
            {
                "_WIN32",
                "_WIN32_WINNT=0x0501"
            } 

            links 
            {

            } 

        filter "system:not windows" 
            links 
            {
                --"pthread"
            }
end
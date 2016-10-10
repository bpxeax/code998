function genCoreProject()
    project "core" 
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
    project "*"
end
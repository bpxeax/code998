premake.cleans = {}
premake.addCleanAction = function(clean_func)
    table.insert(premake.cleans, clean_func)
end

newaction {
    trigger = "clean",
    description = "clean all binaries and generated files",

    onStart = function()
        print("Starting clean...")
    end,

    onWorkspace = function(wks)
        
    end,

    onProject = function(prj)
        
    end,

    execute = function()
        table.foreach(premake.cleans, function(key, clean_fun)
            clean_fun()
        end)
    end,

    onEnd = function()
        print("finish clean")
    end
}
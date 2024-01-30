project "SDL-Music"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "src/audio.h",
        "src/audio.c"
    }

    includedirs
    {
        "src", 
        "%{IncludeDir.SDL2}"
    } 

	links 
	{ 
		"SDL2"
	}
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
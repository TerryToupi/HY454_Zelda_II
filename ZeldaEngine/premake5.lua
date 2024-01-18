project "ZeldaEngine"
	kind "StaticLib" 
	language "C++"
	cppdialect "C++20"
	staticruntime "off" 

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}") 

	files
	{ 
		"src/**.h",
		"src/**.cpp", 
		"src/**.hpp",
	} 
	
	includedirs
	{  
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.json}"
	} 

	links
	{ 
		 "SDL2",
		 "Glad",
		 "opengl32.lib"
	}  

	flags { "NoPCH" }

	filter "system:windows"  

		staticruntime	"on" 
		systemversion	"latest" 

		defines 
		{
			"ENGINE_PLATFORM_WINDOWS"
		} 

	filter "configurations:Debug" 
		defines "ENGINE_DEBUG"
		runtime "Debug"
		symbols "on" 


	filter "configurations:Release" 
		defines "ENGINE_RELEASE" 
		runtime "Release"
		optimize "on"   


workspace "HY454_Zelda_II" 

	architecture "x64"  
	startproject "ZeldaApplication"

	configurations
	{ 
		"Debug",
		"Release"
	} 


outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ZeldaEngine"

	location	"ZeldaEngine" 
	kind		"StaticLib" 
	cppdialect	"C++17" 
	language	"C++"   

	targetdir	("bin/" .. outputDir .. "/%{prj.name}")
	objdir		("bin-int/" .. outputDir .. "/%{prj.name}") 

	files 
	{
		"%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
	}

	includedirs 
	{ 	
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
	}
 
	filter "system:windows"  

		staticruntime	"on" 
		systemversion	"latest" 

		defines 
		{
			"ENGINE_PLATFORM_WINDOWS",
			"ENGINE_BUILD_DLL"
		} 

	filter "configurations:Debug" 
		defines "ENGINE_DEBUG" 
		symbols "on" 


	filter "configurations:Release" 
		defines "ENGINE_RELEASE" 
		optimize "on" 



project "ZeldaApplication" 

	location	"ZeldaApplication" 
	kind		"ConsoleApp" 
	cppdialect	"C++17" 
	language	"C++"   

	targetdir	("bin/" .. outputDir .. "/%{prj.name}")
	objdir		("bin-int/" .. outputDir .. "/%{prj.name}") 

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs 
	{ 
		"ZeldaEngine/vendor/spdlog/include",
		"ZeldaEngine/src",
	}  

	links 
	{ 
		"ZeldaEngine"
	}
 
	filter "system:windows"  

		staticruntime	"on" 
		systemversion	"latest" 

		defines 
		{
			"ENGINE_PLATFORM_WINDOWS",
		} 
		
	filter "configurations:Debug" 
		defines "ENGINE_DEBUG" 
		symbols "on" 


	filter "configurations:Release" 
		defines "ENGINE_RELEASE" 
		optimize "on" 

project "ZeldaUnitests" 
	kind		"ConsoleApp" 
	cppdialect	"C++20" 
	language	"C++"   

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp", 
		"src/**.hpp"
	}

	includedirs 
	{ 
		"%{wks.location}/ZeldaEngine/vendor/spdlog/include",
		"%{wks.location}/ZeldaEngine/src",

		"%{IncludeDir.SDL2}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.json}", 
		"%{IncludeDir.Music}"
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

	postbuildcommands 
	{
	  "{COPY} %{DLLbuildDir.SDL2}/Debug-windows-x86_64/SDL2.dll %{wks.location}/bin/" .. outputDir .. "/ZeldaUnitests"
	}
		
	filter "configurations:Debug" 
		defines "ENGINE_DEBUG" 
		runtime "Debug"
		symbols "on" 


	filter "configurations:Release" 
		defines "ENGINE_RELEASE" 
		runtime "Release"
		optimize "on" 
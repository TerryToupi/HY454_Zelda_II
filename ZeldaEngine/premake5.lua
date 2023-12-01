project "ZeldaEngine"
	kind "StaticLib" 
	language "C++"
	cppdialect "C++17"
	staticruntime "off" 

	targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}") 

	files
	{ 
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"
	} 
	
	includedirs
	{  
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}"
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

		postbuildcommands 
		{
		  "{COPY} %{DLLbuildDir.SDL2}/Debug-windows-x86_64/SDL2.dll %{wks.location}/bin/" .. outputDir .. "/ZeldaApplication"
		}

	filter "configurations:Debug" 
		defines "ENGINE_DEBUG"
		runtime "Debug"
		symbols "on" 


	filter "configurations:Release" 
		defines "ENGINE_RELEASE" 
		runtime "Release"
		optimize "on"   


include "./vendor/premake5/premake_customization/solution_items.lua"

workspace "HY454_Zelda_II" 

	architecture "x64"  
	startproject "ZeldaApplication"

	configurations
	{ 
		"Debug",
		"Release"
	}  

	solution_items
	{ 
		".editorconfig"
	} 

	flags
	{ 
		"MultiProcessorCompile"
	}


outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["SDL2"] = "%{wks.location}/ZeldaEngine/vendor/SDL2/include"
IncludeDir["Glad"] = "%{wks.location}/ZeldaEngine/vendor/Glad/include"
IncludeDir["json"] = "%{wks.location}/ZeldaEngine/vendor/json/single_include/nlohmann" 
IncludeDir["Music"] = "%{wks.location}/ZeldaEngine/vendor/SDL-Music/src"

DLLbuildDir = {} 
DLLbuildDir["SDL2"] = "%{wks.location}/ZeldaEngine/vendor/SDL2/Binaries"

group "Dependencies"
	include "vendor/premake5"
	include "ZeldaEngine/vendor/SDL2/SDL2.lua"
	include "ZeldaEngine/vendor/Glad" 
	include "ZeldaEngine/vendor/SDL-Music"
group ""

group "Engine" 
	include  "ZeldaEngine" 
group "" 

group "Application"
	include  "ZeldaApplication" 
	include	 "ZeldaUnitests"
group "" 

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
IncludeDir["SDL3"] = "%{wks.location}/ZeldaEngine/vendor/SDL/include/SDL3"
IncludeDir["Glad"] = "%{wks.location}/ZeldaEngine/vendor/Glad/include"
IncludeDir["glm"]  = "%{wks.location}/ZeldaEngine/vendor/glm"
IncludeDir["json"] = "%{wks.location}/ZeldaEngine/vendor/json/single_include/nlohmann"

DLLbuildDir = {} 
DLLbuildDir["SDL3"] = "%{wks.location}/ZeldaEngine/vendor/SDL/Binaries"

group "Dependencies"
	include "vendor/premake5"
	include "ZeldaEngine/vendor/SDL/SDL3.lua"
	include "ZeldaEngine/vendor/SDL/SDL3main.lua"
	include "ZeldaEngine/vendor/Glad"
group ""

group "Engine" 
	include  "ZeldaEngine" 
group "" 

group "Application"
	include  "ZeldaApplication" 
	include	 "ZeldaUnitests"
group "" 

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



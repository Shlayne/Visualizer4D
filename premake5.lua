include "Dependencies/premake/Custom/solutionitems.lua"

workspace "Visualizer4D"
	architecture "x86_64"
	startproject "Visualizer4D"

	configurations { "Profile", "Debug", "Release", "Dist" }

	solutionitems {
		-- Visual Studio
		".editorconfig",

		-- Git
		".gitignore",
		".gitattributes",

		-- Scripts
		"Scripts/GenerateProjects.bat",

		-- Lua Scripts
		"premake5.lua",
		"Dependencies/Dependencies.lua",
		"Dependencies/premake/Custom/solutionitems.lua",
		"Dependencies/premake/Custom/usestdpreproc.lua",
		
		-- Misc
		"README.md"
	}

	flags {
		"MultiProcessorCompile"
	}

OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies/premake/Custom/usestdpreproc.lua"
include "Dependencies/Dependencies.lua"

group "Dependencies/Engine"
	include "Engine/Dependencies/glad-0.1.35"
	include "Engine/Dependencies/glfw-3.3.7"
	include "Engine/Dependencies/stb-2.27"
group "Dependencies/Visualizer4D"

group ""

-- Add any projects here with 'include "__PROJECT_NAME__"'
include "Engine"
include "Visualizer4D"

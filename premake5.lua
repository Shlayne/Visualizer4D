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

group "RendererAPI/OpenGL"
	include "Engine/src/Platform/RendererAPI/OpenGL"
	include "Engine/src/Platform/RendererAPI/OpenGL/Dependencies/glad-0.1.35"
--group "RendererAPI/Vulkan"
--	include "Engine/src/Platform/RendererAPI/Vulkan"
group "Dependencies/Engine"
	include "Engine/Dependencies/stb-2.27"
group "Dependencies/Engine/System/Windows"
	include "Engine/Dependencies/glfw-3.3.7"
group "Dependencies/Visualizer4D"

group ""

include "Engine"
include "Visualizer4D"

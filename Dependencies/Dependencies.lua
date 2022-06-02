IncludeDir = {}
LibraryDir = {}
Library = {}

-- Include Directories
	-- Header and Source; they have their own project.
	IncludeDir["glfw"] = "%{wks.location}/Engine/Dependencies/glfw-3.3.7/include"
	IncludeDir["stb"] = "%{wks.location}/Engine/Dependencies/stb-2.27/include"

	-- Header-Only; they don't have their own project.
	IncludeDir["glm"] = "%{wks.location}/Engine/Dependencies/glm-0.9.9.8/include"
	IncludeDir["spdlog"] = "%{wks.location}/Engine/Dependencies/spdlog-1.10.0/include"

	-- Header and Lib; they also don't have their own project.
--	IncludeDir["__LIBRARY_NAME__"] = "%{__LIBRARY_NAME__}/__LIBRARY_INCLUDE_PATH__"

-- Library Directories
--	LibraryDir["__LIBRARY_DIR_NAME__"] = "%{__LIBRARY_DIR_NAME__}/__LIBRARY_DIR_PATH__"

-- Libraries
--	Library["__LIBRARY_NAME__"] = "%{LibraryDir.__LIBRARY_NAME__}/__LIBRARY_PATH__"

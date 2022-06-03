project "Visualizer4D"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	cdialect "C17"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

	pchheader "Visualizer4D/pch.h"
	pchsource "src/Visualizer4D/pch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
		"src/**.inl"
	}

	includedirs {
		"src",

		-- Add any project source directories here.
		"%{wks.location}/Engine/src",

		-- Add any header and source dependency includes here.
		"%{IncludeDir.stb}",

		-- Add any header-only dependency includes here.
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}"
	}

	-- Add any links dependency libs via their project names here.
	links {
		"Engine"
	}

	filter "system:windows"
		systemversion "latest"
		usestdpreproc "On"
-- Until Microsoft updates Windows 10 to not have terrible code, this must be here to prevent a warning.
		buildoptions "/wd5105"

		defines "SYSTEM_WINDOWS"

	filter "configurations:Profile"
		runtime "Debug"
		optimize "Off"
		symbols "On"

		defines {
			"CONFIG_PROFILE",

			"ENABLE_PROFILE",
			"ENABLE_ASSERTS",
			"ENABLE_STATS",
			"ENABLE_LOGGING"
		}

	filter "configurations:Debug"
		runtime "Debug"
		optimize "Debug"
		symbols "Full"

		defines {
			"CONFIG_DEBUG",

			"ENABLE_PROFILE",
			"ENABLE_ASSERTS",
			"ENABLE_STATS",
			"ENABLE_LOGGING"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "On"

		defines {
			"CONFIG_RELEASE",

			"ENABLE_STATS",
			"ENABLE_LOGGING"
		}

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full"
		symbols "Off"

		defines "CONFIG_DIST"

		kind "WindowedApp"

project "Visualizer4D"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	cdialect "C17"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

	pchheader "Visualizer4D/pch.h"
	pchsource "src/Visualizer4D/pch.cpp"

	files {
		"src/**.h",
		"src/**.c",
		"src/**.hpp",
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
		defines "PLATFORM_WINDOWS"
		usestdpreproc "On"
-- Until Microsoft updates Windows 10 to not have terrible code, this must be here to prevent a warning.
		buildoptions "/wd5105"

	filter "configurations:Profile"
		defines {
			"CONFIG_PROFILE",

			"ENABLE_PROFILE",
			"ENABLE_ASSERTS",
			"ENABLE_STATS",
			"ENABLE_LOGGING"
		}
		runtime "Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Debug"
		defines {
			"CONFIG_DEBUG",

			"ENABLE_PROFILE",
			"ENABLE_ASSERTS",
			"ENABLE_STATS",
			"ENABLE_LOGGING"
		}
		runtime "Debug"
		optimize "Debug"
		symbols "Full"

	filter "configurations:Release"
		defines {
			"CONFIG_RELEASE",

			"ENABLE_STATS",
			"ENABLE_LOGGING"
		}
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Dist"
		kind "WindowedApp"
		defines "CONFIG_DIST"
		runtime "Release"
		optimize "Full"
		symbols "Off"

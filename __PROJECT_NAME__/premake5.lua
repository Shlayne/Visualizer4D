project "__PROJECT_NAME__"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	cdialect "C17"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

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
		-- "%{wks.location}/__PROJECT_NAME__/src",

		-- Add any header and source dependency includes here.
		-- "%{IncludeDir.__PROJECT_NAME__}",

		-- Add any header-only dependency includes here.
		-- "%{IncludeDir.__PROJECT_NAME__}",
	}
	
	-- Add any links dependency libs via their project names here.
	links {
		--	"__PROJECT_NAME__"
	}

	filter "system:windows"
		systemversion "latest"
		defines "PLATFORM_WINDOWS"
		usestdpreproc "On"
-- Until Microsoft updates Windows 10 to not have terrible code, this must be here to prevent a warning.
		buildoptions "/wd5105"

	filter "configurations:Debug"
		defines "CONFIG_DEBUG"
		runtime "Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		defines "CONFIG_RELEASE"
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Dist"
		kind "WindowedApp"
		defines "CONFIG_DIST"
		runtime "Release"
		optimize "On"
		symbols "Off"

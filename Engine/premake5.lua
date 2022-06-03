project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	cdialect "C17"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

	pchheader "Engine/pch.h"
	pchsource "src/Engine/pch.cpp"

	files {
		"src/Engine/**.h",
		"src/Engine/**.cpp",
		"src/Engine/**.inl",
		"src/Engine.h"
	}

	includedirs {
		"src",

		-- Add any header and source dependency includes here.
		"%{IncludeDir.glfw}",
		"%{IncludeDir.stb}",

		-- Add any header-only dependency includes here.
		"%{IncludeDir.glm}",
		"%{IncludeDir.spdlog}"
	}

	-- Add any links dependency libs via their project names here.
	links {
		"glfw",
		"stb"
	}

	filter "system:windows"
		systemversion "latest"
		usestdpreproc "On"
-- Until Microsoft updates Windows 10 to not have terrible code, this must be here to prevent a warning.
		buildoptions "/wd5105"
		
		defines {
			"SYSTEM_WINDOWS",
			"SYSTEM_SUPPORTS_OPENGL",
			"SYSTEM_SUPPORTS_VULKAN"
		}

		includedirs {
			-- OpenGL
			"%{IncludeDir.glad}",

			-- TODO: Vulkan
		}

		links {
			-- OpenGL
			"glad",

			-- TODO: Vulkan
		}

		files {
			"src/Platform/System/Windows/**.h",
			"src/Platform/System/Windows/**.cpp",
			"src/Platform/System/Windows/**.inl",

			"src/Platform/Renderer/OpenGL/**.h",
			"src/Platform/Renderer/OpenGL/**.cpp",
			"src/Platform/Renderer/OpenGL/**.inl",

			--"src/Platform/Renderer/Vulkan/**.h",
			--"src/Platform/Renderer/Vulkan/**.cpp",
			--"src/Platform/Renderer/Vulkan/**.inl",
		}

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

		excludes {
			"src/Engine/Debug/**.h",
			"src/Engine/Debug/**.cpp",
			"src/Engine/Debug/**.inl",
		}

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full"
		symbols "Off"

		defines "CONFIG_DIST"

		excludes {
			"src/Engine/Debug/**.h",
			"src/Engine/Debug/**.cpp",
			"src/Engine/Debug/**.inl",
		}

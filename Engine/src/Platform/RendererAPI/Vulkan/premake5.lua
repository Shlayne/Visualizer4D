project "VulkanRendererAPI"
	filter "system:not windows"
		kind "None"

	filter "system:windows"
		kind "SharedLib"
		language "C++"
		cppdialect "C++20"
		cdialect "C17"
		staticruntime "Off"

		targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
		objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

		postbuildcommands {
			"xcopy \"%{wks.location}bin\\" .. OutputDir .. "\\VulkanRendererAPI\\VulkanRendererAPI.dll\" \"%{wks.location}bin\\" .. OutputDir .. "\\Visualizer4D\\\" /Y"
		}

		defines "VULKAN_RENDERER_API_EXPORT"

		files {
			"src/**.h",
			"src/**.cpp",
			"src/**.inl"
		}

		includedirs {
			-- Add any project source directories here.
			"src",
			"%{wks.location}/Engine/src",

			-- Add any dependency includes here.
			"%{IncludeDir.glm}",
			"%{IncludeDir.spdlog}",
			"%{IncludeDir.stb}",
			"%{IncludeDir.VulkanSDK}",
		}

		libdirs "%{LibraryDir.VulkanSDK}"
		links "%{Library.VulkanSDK}"

		filter "system:windows"
			systemversion "latest"
			usestdpreproc "On"
	-- Until Microsoft updates Windows 10 to not have terrible code, this must be here to prevent a warning.
			buildoptions "/wd5105"

			defines "SYSTEM_WINDOWS"

			includedirs {
				"%{IncludeDir.glfw}"
			}

		filter { "system:windows", "configurations:Profile" }
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

		filter { "system:windows", "configurations:Debug" }
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

		filter { "system:windows", "configurations:Release" }
			runtime "Release"
			optimize "On"
			symbols "On"

			defines {
				"CONFIG_RELEASE",

				"ENABLE_STATS",
				"ENABLE_LOGGING"
			}

		filter { "system:windows", "configurations:Dist" }
			runtime "Release"
			optimize "Full"
			symbols "Off"

			defines "CONFIG_DIST"

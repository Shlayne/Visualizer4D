project "glfw"
	kind "StaticLib"
	language "C"
	cdialect "C17"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

	files {
		"include/glfw/glfw3.h",
		"include/glfw/glfw3native.h",
		"src/context.c",
		"src/init.c",
		"src/input.c",
		"src/monitor.c",
		"src/window.c"
	}

	filter "system:windows"
		systemversion "latest"

		files {
			"src/win32_init.c",
			"src/win32_joystick.c",
			"src/win32_monitor.c",
			"src/win32_time.c",
			"src/win32_thread.c",
			"src/win32_window.c",

			"src/wgl_context.c",
			"src/egl_context.c",
			"src/osmesa_context.c",

			"src/vulkan.c"
		}

		defines {
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Profile"
		runtime "Debug"
		optimize "Off"
		symbols "On"

	filter "configurations:Debug"
		runtime "Debug"
		optimize "Debug"
		symbols "Full"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full"
		symbols "Off"

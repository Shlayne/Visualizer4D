VULKAN_SDK = os.getenv("VULKAN_SDK")

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
	IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

-- Library Directories
	LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib/"
	LibraryDir["VulkanSDK_Debug"] = "%{VULKAN_SDK}/Debug/Lib/"

-- Libraries
	Library["VulkanSDK"] = "vulkan-1"

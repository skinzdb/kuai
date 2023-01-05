workspace "Smongine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Smongine/vendor/Glad"
-- include "Smongine/vendor/ImGui"
include "Smongine/vendor/GLFW"

project "Smongine"
	location "Smongine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	pchheader "smpch.h"
	pchsource "Smongine/src/smpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/stb_image/stb_image.h",
		"%{prj.name}/vendor/stb_image/stb_image.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/Glad/include",
		-- "%{prj.name}/vendor/ImGui",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/stb_image"
	}

	links
	{
		"GLFW",
		"Glad",
		-- "ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"SM_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "SMONG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SMONG_RELEASE"
		runtime "Release"
		optimize "on"	
	
	filter "configurations:Dist"
		defines "SMONG_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox" 
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Smongine/vendor/spdlog/include",
		"Smongine/src",
		"Smongine/vendor",
		"Smongine/vendor/glm"
	}

	links
	{
		"Smongine",
		"dwmapi.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SMONG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SMONG_RELEASE"
		runtime "Release"
		optimize "on"	
	
	filter "configurations:Dist"
		defines "SMONG_DIST"
		runtime "Release"
		optimize "on"
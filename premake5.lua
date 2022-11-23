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
include "Smongine/vendor/ImGui"

project "Smongine"
	location "Smongine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "smpch.h"
	pchsource "Smongine/src/smpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include",
		"%{prj.name}/vendor/Glad/include",
		"%{prj.name}/vendor/ImGui"
	}

	libdirs
	{
		"%{prj.name}/vendor/GLFW/lib-vc2019"
	}

	links
	{
		"glfw3.lib",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"SM_PLATFORM_WINDOWS",
			"SM_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}


	filter "configurations:Debug"
		defines "SM_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SM_RELEASE"
		optimize "On"	
	
	filter "configurations:Dist"
		defines "SM_DIST"
		optimize "On"

project "Sandbox" 
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Smongine/vendor/spdlog/include",
		"Smongine/src"
	}

	links
	{
		"Smongine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"SM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SM_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SM_RELEASE"
		optimize "On"	
	
	filter "configurations:Dist"
		defines "SM_DIST"
		optimize "On"
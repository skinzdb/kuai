workspace "kuai"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "kuai/vendor/Glad"
include "kuai/vendor/GLFW"

project "kuai"
	location "kuai"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	pchheader "kpch.h"
	pchsource "kuai/src/kpch.cpp"

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
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/stb_image",
		--"%{prj.name}/vendor/mono/include",
		"%{prj.name}/vendor/openal-soft/include",
		"%{prj.name}/vendor/libsndfile/include",
		"%{prj.name}/vendor/assimp/include"
	}

	libdirs
	{
		--"%{prj.name}/vendor/mono/lib",
		"%{prj.name}/vendor/openal-soft/build/Debug",
		"%{prj.name}/vendor/libsndfile/build/Debug",
		"%{prj.name}/vendor/assimp/build/lib/Debug",
		"%{prj.name}/vendor/assimp/contrib/zlib/Debug"
	}

	links
	{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"OpenAL32.lib",
		"sndfile.lib",

		--"libmono-static-sgen.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
			"KU_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		libdirs
		{
			"%{prj.name}/vendor/openal-soft/build/Debug",
			"%{prj.name}/vendor/libsndfile/build/Debug",
			"%{prj.name}/vendor/assimp/build/lib/Debug",
			"%{prj.name}/vendor/assimp/contrib/zlib/Debug"
		}

		links
		{
			"assimp-vc142-mtd.lib",
			"zlibstaticd.lib"	-- building assimp statically requires you to include zlib too
		}

		defines "KU_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		libdirs
		{
			"%{prj.name}/vendor/openal-soft/build/Release",
			"%{prj.name}/vendor/libsndfile/build/Release",
			"%{prj.name}/vendor/assimp/build/lib/Release",
			"%{prj.name}/vendor/assimp/contrib/zlib/Release"
		}

		links
		{
			"assimp-vc142-mt.lib",
			"zlibstatic.lib"	-- building assimp statically requires you to include zlib too
		}

		defines "KU_RELEASE"
		runtime "Release"
		optimize "on"	
	
	filter "configurations:Dist"
		defines "KU_DIST"
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
		--"kuai/vendor/pybind11/include",
		
		"kuai/vendor/spdlog/include",
		"kuai/src",
		"kuai/vendor",
		"kuai/vendor/glm"
	}

	links
	{
		"kuai",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"KU_PLATFORM_WINDOWS"
		}

		links
		{
			"dwmapi.lib",
			"winmm.lib"	-- For audio
		}

	filter "configurations:Debug"
		defines "KU_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "KU_RELEASE"
		runtime "Release"
		optimize "on"	
	
	filter "configurations:Dist"
		defines "KU_DIST"
		runtime "Release"
		optimize "on"

project "PythonBinder"
	location "PythonBinder"
	kind "SharedLib"
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
		"kuai/vendor/spdlog/include",
		"kuai/src",
		"kuai/vendor",
		"kuai/vendor/glm"
	}

	links
	{
		"kuai",
		"dwmapi.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "KU_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "KU_RELEASE"
		runtime "Release"
		optimize "on"	

	filter "configurations:Dist"
		defines "KU_DIST"
		runtime "Release"
		optimize "on"

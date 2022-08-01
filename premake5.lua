workspace "RefraEngine"
	architecture "x64"
	startproject "Sandbox"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories relative to root folder (solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Engine/vendor/Glad/include"
IncludeDir["ImGui"] = "Engine/vendor/imgui"
IncludeDir["glm"] = "Engine/vendor/glm"

group "Dependencies"
	include "Engine/vendor/GLFW"
	include "Engine/vendor/Glad"
	include "Engine/vendor/imgui"

group ""
--END

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermidiate/" .. outputdir .. "/%{prj.name}")
	
	pchheader "rfpch.h"
	pchsource "Engine/src/rfpch.cpp"
	
	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}
	
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		
		defines
		{
			"RF_PLATFORM_WINDOWS",
			"RF_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}
		
		filter "configurations:Debug"
			defines "RF_DEBUG"
			runtime "Debug"
			symbols "On"
			
		filter "configurations:Release"
			defines "RF_RELEASE"
			runtime "Release"
			optimize "On"
		
		filter "configurations:Dist"
			defines "RF_DIST"
			runtime "Release"
			optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/intermidiate/" .. outputdir .. "/%{prj.name}")
	
	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Engine/vendor/spdlog/include",
		"Engine/src",
		"Engine/vendor",
		"%{IncludeDir.glm}"
	}
	
	links
	{
		"Engine"
	}
	
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		
		defines
		{
			"RF_PLATFORM_WINDOWS"
		}
		
		filter "configurations:Debug"
			defines "RF_DEBUG"
			runtime "Debug"
			symbols "On"
			
		filter "configurations:Release"
			defines "RF_RELEASE"
			runtime "Release"
			optimize "On"
		
		filter "configurations:Dist"
			defines "RF_DIST"
			runtime "Release"
			optimize "On"
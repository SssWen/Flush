workspace "Flush"
	architecture "x64"
	startproject "Sandbox"
	configurations
	{
		"Debug",
		"Release",
	}

	--$(SolutionDir) = E:\Code\FlushEngine\Flush\
	--$(Configurations) = Debug
	--$(Platform) = x64
	--$(ProjectName) = Sandbox

	

-- outputdir = "%{cfg.buildcfg}-%{cfg.system}%{cfg.architecture}"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] 			= "Flush/vendor/GLFW/include"
IncludeDir["glad"] 			= "Flush/vendor/glad/include"
IncludeDir["ImGui"] 		= "Flush/vendor/imgui"
IncludeDir["glm"] 			= "Flush/vendor/glm"
IncludeDir["stb_image"] 	= "Flush/vendor/stb_image"

group "Dependencies"
	include "Flush/vendor/GLFW" -- 另一个项目的premake
	include "Flush/vendor/glad" 
	include "Flush/vendor/imgui" 

group ""	

project "Flush"
	location "Flush"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"	
	staticruntime "on"		

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


	pchheader "flushpch.h"
	pchsource "Flush/src/flushpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/**.hpp",
		"%{prj.name}/vendor/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
	}
	defines
	{
		"FLUSH_PLATFORM_WINDOW", -- use windows plantform
		"_CRT_SECURE_NO_WARNINGS", -- skip secure warning 		
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{prj.name}/src/Flush",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{prj.name}/vendor/assimp/include",
	}

	links
	{		
		"GLFW",
		"glad",
		"ImGui",
		"opengl32.lib"
	}
	filter "system:windows"			
		systemversion "latest"
		-- postbuildcommands -- 不在使用dll了,所以不进行copy dll
		-- {			
		-- 	("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		-- }

	filter "configurations:Debug"
		defines "FLUSH_DEBUG"		
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "FLUSH_RELEASE"
		runtime "Release"		
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	--targetdir: sets the destination dir for the compiled binary target.
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Flush/vendor/spdlog/include",
		"Flush/src",
		"Flush/src/Flush",
		"Flush/vendor",		
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGui}",		
		"%{IncludeDir.stb_image}",
	}

	links
	{
		"Flush"
	}

	filter "system:windows"						
        systemversion "latest"
		defines
		{
			"FLUSH_PLATFORM_WINDOW", -- 使用windows平台
		}

	filter "configurations:Debug"
		defines "FLUSH_DEBUG"	
		runtime "Debug"			
		symbols "On"

	filter "configurations:Release"
		defines "FLUSH_RELEASE"			
		optimize "On"


project "FlushPBREditor"
		location "FlushPBREditor"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"
		
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
		links 
		{ 
			"Flush"
		}
		
		files 
		{ 
			"%{prj.name}/src/**.h", 
			"%{prj.name}/src/**.c", 
			"%{prj.name}/src/**.hpp", 
			"%{prj.name}/src/**.cpp" 
		}
	
		includedirs
		{
			"Flush/vendor/spdlog/include",
			"Flush/src",
			"Flush/src/Flush",
			"Flush/vendor",		
			"%{IncludeDir.glm}",
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.glad}",
			"%{IncludeDir.ImGui}",		
			"%{IncludeDir.stb_image}",
		}
	
		postbuildcommands 
		{
			'{COPY} "../FlushPBREditor/assets" "%{cfg.targetdir}/assets"'
		}
		
		filter "system:windows"
			systemversion "latest"
					
			defines 
			{ 
				"FLUSH_PLATFORM_WINDOW"
			}
		
		filter "configurations:Debug"
			defines "HZ_DEBUG"
			symbols "on"
	
			links
			{
				"Flush/vendor/assimp/bin/Debug/assimp-vc141-mtd.lib"
			}
	
			postbuildcommands 
			{
				'{COPY} "../Flush/vendor/assimp/bin/Debug/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
			}
					
		filter "configurations:Release"
			defines "FLUSH_RELEASE"
			optimize "on"
	
			links
			{
				"Flush/vendor/assimp/bin/Release/assimp-vc141-mt.lib"
			}
	
			postbuildcommands 
			{
				'{COPY} "../Flush/vendor/assimp/bin/Release/assimp-vc141-mtd.dll" "%{cfg.targetdir}"'
			}
	
	
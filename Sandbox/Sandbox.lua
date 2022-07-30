project "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
        "%{wks.location}/ShapeGenerator/src",
		"%{wks.location}/Teapot/Teapot/src",
		"%{wks.location}/Teapot/Teapot/vendor/GLFW/include"
	}

	links
	{
		"Teapot",
        "ShapeGenerator"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"TEA_PLATFORM_WINDOWS"
		}
	
	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		links 
		{
			"GLFW", "Xrandr", "Xi", "GLU", "GL", "X11", "dl", "pthread", "stdc++fs"
		}

		defines
		{
			"TEA_PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines "TEA_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "TEA_RELEASE"
		optimize "On"
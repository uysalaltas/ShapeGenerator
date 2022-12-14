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
		"%{wks.location}/ShapeGenerator/vendor/glm",
		"%{wks.location}/vendor/Teapot/Teapot/src",
		"%{wks.location}/vendor/Teapot/Teapot/vendor/GLFW/include",
		"%{wks.location}/vendor/Teapot/Teapot/vendor/GLAD/include",
		"%{wks.location}/vendor/Teapot/Teapot/vendor/ImGui",
		"%{wks.location}/vendor/Teapot/Teapot/vendor/stb_image",
		"%{wks.location}/vendor/Teapot/Teapot/vendor/assimp/include"
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
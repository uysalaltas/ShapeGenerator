project "ShapeGenerator"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"src",
		"%{wks.location}/vendor/Teapot/Teapot/vendor/GLAD/include",
		"vendor/glm"

	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "system:linux"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines "TEA_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "TEA_RELEASE"
		optimize "On"
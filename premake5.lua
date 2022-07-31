workspace "ShapeGenerator"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "vendor/Teapot/TeapotExternal.lua"
include "Sandbox/Sandbox.lua"

group "Core"
    include "ShapeGenerator/ShapeGenerator.lua"
group ""
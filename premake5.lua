-- Set up the workspace
workspace (path.getbasename(os.getcwd()))
    configurations { "Debug", "DebugApp", "Development", "Test", "Shipping" }
    platforms { "Win32", "Win64" }
    location "ProjectFiles" -- Specify the location for generated project files
    startproject "Application"
    defines { "GLEW_STATIC" }

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x86"

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x86_64"

    filter "system:windows"
        systemversion "latest"
        defines { "PLATFORM_DESKTOP" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "Full"
        optimize "Off"

    filter "configurations:Development"
        runtime "Release"
        symbols "On"
        optimize "On"

    filter "configurations:Test"
        runtime "Release"
        symbols "Off"
        optimize "On"

    filter "configurations:Shipping"
        runtime "Release"
        symbols "Off"
        optimize "Full"


-- GLFW Project
project "GLFW"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    targetdir ("Build/%{cfg.buildcfg}/%{cfg.platform}/")
    objdir ("Intermediate/%{cfg.buildcfg}/%{cfg.platform}/") -- Specify the location for intermediate files

    files
    {
        "GLFW/include/GLFW/*.h",
        "GLFW/src/*.c"
    }

    includedirs
    {
        "GLFW/include"
    }

    filter "system:windows"
        defines { "_GLFW_WIN32" }

    filter "configurations:DebugApp"
        runtime "Debug"
        symbols "Off"
        optimize "On"


-- GLEW Project
project "GLEW"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    targetdir ("Build/%{cfg.buildcfg}/%{cfg.platform}/")
    objdir ("Intermediate/%{cfg.buildcfg}/%{cfg.platform}/") -- Specify the location for intermediate files
    defines { "GLEW_STATIC" }

    files
    {
        "GLEW/include/GL/*.h",
        "GLEW/src/*.c"
    }

    includedirs
    {
        "GLEW/include"
    }

    filter "configurations:DebugApp"
        runtime "Debug"
        symbols "Off"
        optimize "On"


-- App Project
project "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("Build/%{cfg.buildcfg}/%{cfg.platform}/")
    objdir ("Intermediate/%{cfg.buildcfg}/%{cfg.platform}/") -- Specify the location for intermediate files

    files
    {
        "Application/Include/**.h",
        "Application/Source/**.cpp"
    }

    includedirs
    {
        "Application/Include",
        "GLFW/include",
        "GLEW/include"
    }

    links
    {
        "GLFW",
        "GLEW",
        "opengl32" -- Link the OpenGL library
    }

    libdirs
    {
        "Build/%{cfg.buildcfg}/%{cfg.platform}/GLFW",
        "Build/%{cfg.buildcfg}/%{cfg.platform}/GLEW",
        "Build/%{cfg.buildcfg}/%{cfg.platform}/",
    }

    vpaths
    {
        ["Header Files"] = { "Application/Include/**.h" },
        ["Source Files"] = { "Application/Source/**.cpp" }
    }

    filter "configurations:DebugApp"
        runtime "Debug"
        symbols "On"
        optimize "Off"

require "vscode"

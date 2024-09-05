-- Set up the workspace
workspace (path.getbasename(os.getcwd())) -- Get the current working directory and use its name as the workspace name
    configurations { "Debug", "DebugApp", "Development", "Test", "Shipping" } -- Define build configurations
    platforms { "Win32", "Win64" } -- Define target platforms
    location "ProjectFiles" -- Specify the location for generated project files
    startproject "Application" -- Set the default startup project
    defines { "GLEW_STATIC" } -- Preprocessor definitions for all projects

    filter { "platforms:Win32" } -- Filter settings for Win32 platform
        system "Windows"
        architecture "x86"

    filter { "platforms:Win64" } -- Filter settings for Win64 platform
        system "Windows"
        architecture "x86_64"

    filter "system:windows" -- Filter settings for Windows system
        systemversion "latest" -- Use the latest Windows SDK
        defines { "PLATFORM_DESKTOP" }

    filter "configurations:Debug" -- Filter settings for Debug configuration
        runtime "Debug"
        symbols "Full"
        optimize "Off"

    filter "configurations:Development" -- Filter settings for Development configuration
        runtime "Release"
        symbols "On"
        optimize "On"

    filter "configurations:Test" -- Filter settings for Test configuration
        runtime "Release"
        symbols "Off"
        optimize "On"

    filter "configurations:Shipping" -- Filter settings for Shipping configuration
        runtime "Release"
        symbols "Off"
        optimize "Full"


-- Project configuration for GLFW, a multi-platform library for OpenGL, window and input
project "GLFW"
    kind "StaticLib" -- Static library, not an executable or DLL
    language "C" -- Programming language used is C
    cdialect "C11" -- C standard dialect to use is C11
    targetdir ("Build/%{cfg.buildcfg}/%{cfg.platform}/") -- Output directory for build target relative to project root
    objdir ("Intermediate/%{cfg.buildcfg}/%{cfg.platform}/") -- Intermediate files directory relative to project root

    -- The rest of GLFW project configuration specifies files to include, directories, and platform-specific settings

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


-- Project configuration for GLEW, The OpenGL Extension Wrangler Library
project "GLEW"
-- Similar setup as GLFW but with GLEW-specific paths and files
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



-- Project configuration for the main application that will use GLFW and GLEW libraries
project "Application"
-- Setup includes kind of project, language, directories, files, links to libraries, and other configurations
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir ("Build/%{cfg.buildcfg}/%{cfg.platform}/")
    objdir ("Intermediate/%{cfg.buildcfg}/%{cfg.platform}/") -- Specify the location for intermediate files

    files
    {
        "Application/Include/**.h",
        "Application/Source/**.cpp",
        "Application/Resources/**/"
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
        ["Source Files"] = { "Application/Source/**.cpp" },
        ["Resources Files"] = {"Application/Resources/**"}
    }

    filter "configurations:DebugApp"
        runtime "Debug"
        symbols "On"
        optimize "Off"

-- Include Visual Studio Code integration which might be used for tasks like building or debugging within VS Code editor environment.
require("vscode")

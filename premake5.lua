-- Set up the workspace
workspace (path.getbasename(os.getcwd())) -- Get the current working directory and use its name as the workspace name
    configurations { "Debug", "Release" } -- Define build configurations
    --platforms { "Win32", "Win64", "Linux" } -- Add Linux to target platforms

    if os.target() == "windows" then
        platforms { "Win32", "Win64" }
    elseif os.target() == "linux" then
        platforms { "Linux" }
       -- buildoptions { "-gdwarf-4" }
       -- linkoptions { "-fuse-ld=lld" }
    end
    
    --location "ProjectFiles" -- Specify the location for generated project files
    startproject "Application" -- Set the default startup project
    defines { "GLEW_STATIC" } -- Preprocessor definitions for all projects

    filter { "platforms:Win32" } -- Filter settings for Win32 platform
        system "Windows"
        architecture "x86"

    filter { "platforms:Win64" } -- Filter settings for Win64 platform
        system "Windows"
        architecture "x86_64"

    filter { "platforms:Linux" } -- Filter settings for Linux platform
        system "Linux"
        architecture "x86_64"
        toolset "clang" -- Use Clang as the compiler

    filter "system:windows" -- Filter settings for Windows system
        systemversion "latest" -- Use the latest Windows SDK
        defines { "PLATFORM_DESKTOP", "WINDOWS" }
        links { "opengl32" }

    filter "system:linux" -- Filter settings for Linux system
        buildoptions { "-Wall", "-Wextra", "-Wpedantic", "-Werror" } -- Enable warnings and treat them as errors
        defines { "PLATFORM_DESKTOP", "LINUX" }
        links { "pthread", "dl", "X11", "GL" } -- Link standard Linux libraries

    filter "configurations:Debug" -- Filter settings for Debug configuration
        runtime "Debug"
        defines { "DEBUG" }
        symbols "Full"
        optimize "Off"

    filter "configurations:Release" -- Filter settings for Release configuration
        runtime "Release"
        defines { "RELEASE" }
        symbols "Off"
        optimize "Full"



-- Project configuration for GLFW, a multi-platform library for OpenGL, window and input
project "GLFW"
    kind "StaticLib" -- Static library, not an executable or DLL
    language "C" -- Programming language used is C
    cdialect "C11" -- C standard dialect to use is C11
    targetdir ("Build/%{cfg.buildcfg}/%{cfg.platform}/") -- Output directory for build target relative to project root
    objdir ("Intermediate/%{cfg.buildcfg}/%{cfg.platform}/") -- Intermediate files directory relative to project root

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

    filter "system:linux"
        defines { "_GLFW_X11" }


-- Project configuration for GLEW, The OpenGL Extension Wrangler Library
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

    filter "system:linux"
        links { "GL" } -- Link OpenGL for Linux


-- Project configuration for the main application that will use GLFW and GLEW libraries
project "Application"
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
        "GLEW"
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

    filter "system:linux"
        links { "X11", "pthread", "dl", "GL", "glfw"} -- Link Linux-specific libraries

    filter "system:windows"
        links { "opengl32" }
-- Set up the workspace
workspace (path.getbasename(os.getcwd()))
    configurations { "Debug", "Release" }
    platforms { "Win32", "Win64"}
    --toolset ("clang")
    location "ProjectFiles" -- Specify the location for generated project files
    startproject "Application"
    defines { "GLEW_STATIC" }  

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x86"

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x86_64"

      

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
        systemversion "latest"
        defines { "_GLFW_WIN32" }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "Full"


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

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "Full"        


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

    filter "system:windows"
        systemversion "latest"
        defines { "PLATFORM_DESKTOP" }

    filter "configurations:Debug"
        runtime "Debug"
        optimize "Off"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "Full"

        
require "vscode"  
-- Змінні для директорій
local targetDir = "Build/%{cfg.buildcfg}/%{cfg.platform}/"
local objDir = "Intermediate/%{cfg.buildcfg}/%{cfg.platform}/"
local projectFilesDir = "ProjectFiles/"
local thirdPartyDir = "ThirdParty/"
local applicationDir = "Application/"

-- Set up the workspace
workspace (path.getbasename(os.getcwd())) -- Get the current working directory and use its name as the workspace name
    configurations { "Debug", "Release" } -- Define build configurations
    if os.target() == "windows" then
        platforms { "Win32", "Win64" }
    elseif os.target() == "linux" then
        platforms { "Linux" }
    end
    
    toolset "clang"  -- Встановлюємо Clang як інструментарій компіляції

    require("premake-vscode")

    location (projectFilesDir)
    startproject "Application" -- Set the default startup project
    defines { "GLEW_STATIC" } -- Preprocessor definitions for all projects

    filter { "platforms:Win32" }
        system "Windows"
        architecture "x86"

    filter { "platforms:Win64" }
        system "Windows"
        architecture "x86_64"

    filter { "platforms:Linux" }
        system "Linux"
        architecture "x86_64"
        toolset "clang"

    filter "system:windows"
        systemversion "latest"
        defines { "PLATFORM_DESKTOP", "WINDOWS" }
        links { "opengl32" }

    filter "system:linux"
        buildoptions { "-Wall", "-Wextra", "-Wpedantic", "-Werror" }
        defines { "PLATFORM_DESKTOP", "LINUX" }
        links { "pthread", "dl", "X11", "GL" }

    filter "configurations:Debug"
        runtime "Debug"
        defines { "DEBUG" }
        symbols "Full"
        optimize "Off"

    filter "configurations:Release"
        runtime "Release"
        defines { "RELEASE" }
        symbols "Off"
        optimize "Full"

-- Project configuration for GLFW
project "GLFW"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    targetdir (targetDir)
    objdir (objDir)

    files
    {
        thirdPartyDir.."GLFW/include/GLFW/*.h",
        thirdPartyDir.."GLFW/src/*.c"
    }

    includedirs
    {
        thirdPartyDir.."GLFW/include"
    }

    filter "system:windows"
        defines { "_GLFW_WIN32" }

    filter "system:linux"
        defines { "_GLFW_X11" }

-- Project configuration for GLEW
project "GLEW"
    kind "StaticLib"
    language "C"
    cdialect "C11"
    targetdir (targetDir)
    objdir (objDir)
    defines { "GLEW_STATIC" }

    files
    {
        thirdPartyDir.."GLEW/include/GL/**.h",
        thirdPartyDir.."GLEW/src/**.c"
    }

    includedirs
    {
        thirdPartyDir.."GLEW/include"
    }

    filter "system:linux"
        links { "GL" }

-- Project configuration for ImGui
project "imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir (targetDir)
    objdir (objDir)

    files
    {
        thirdPartyDir.."imgui/include/**.h",
        thirdPartyDir.."imgui/include/**.cpp"
    }

    includedirs
    {
        thirdPartyDir.."imgui/include",
        thirdPartyDir.."GLFW/include",
        thirdPartyDir.."GLEW/include"
    }

    links
    {
        "GLFW",
        "GLEW"
    }

-- Project configuration for GLM
project "glm"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir (targetDir)
    objdir (objDir)

    files
    {
        thirdPartyDir.."glm/glm/**.hpp",
        thirdPartyDir.."glm/glm/**.inl"
    }

    includedirs
    {
        thirdPartyDir.."glm"
    }

-- Project configuration for the main application
project "Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir (targetDir)
    objdir (objDir)

    files
    {
        applicationDir.."Include/**.h",
        applicationDir.."Source/**.cpp",
        applicationDir.."Resources/**"
    }

    includedirs
    {
        applicationDir.."Include",
        thirdPartyDir.."GLFW/include",
        thirdPartyDir.."GLEW/include",
        thirdPartyDir.."imgui/include",
        thirdPartyDir.."glm"
    }

    links
    {
        "GLFW",
        "GLEW",
        "imgui"
    }

    libdirs
    {
        targetDir.."GLFW",
        targetDir.."GLEW",
        targetDir.."imgui",
    }

    vpaths
    {
        ["Header Files"] = { applicationDir.."Include/**.h" },
        ["Source Files"] = { applicationDir.."Source/**.cpp" },
        ["Resources Files"] = { applicationDir.."Resources/**" }
    }

    filter "system:windows"
        links { "opengl32" }

    filter "system:linux"
        links { "X11", "pthread", "dl", "GL", "glfw" }

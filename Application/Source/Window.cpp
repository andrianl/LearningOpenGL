#include "Window.h"

void OpenGLVersionInit(int Major, int Minor) 
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
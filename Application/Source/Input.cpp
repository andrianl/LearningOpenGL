#include "Input.h"
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int ChangePolygonMode(GLFWwindow* window)
{
    static bool Fill = true;
    static int mode = GL_FILL;
    static bool keyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !keyPressed)
    {
        keyPressed = true;
        Fill = !Fill;
        mode = Fill ? GL_FILL : GL_LINE;
    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        keyPressed = false;
    }

    return mode;
}
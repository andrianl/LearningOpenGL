#include "Input.h"
#include <GLFW/glfw3.h>

// Визначення статичного члена класу поза межами класу
std::unordered_map<int, Delegate> InputManager::keyDelegates;
InputManager* InputManager::currentInstance = nullptr;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int ChangePolygonMode(GLFWwindow* window)
{
    static bool Fill = true;
    static int mode = GL_FILL;

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        Fill = !Fill;
        mode = Fill ? GL_FILL : GL_LINE;
    }

    glPolygonMode(GL_FRONT_AND_BACK, mode);

    return mode;
}

InputManager::InputManager(GLFWwindow* window_ptr)
{
    window = window_ptr;
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    currentInstance = this;
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
}
void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (keyDelegates.find(key) != keyDelegates.end())
        {
            auto results = keyDelegates[key].Broadcast<int, GLFWwindow*>(window);
            for (int result : results)
            {
                std::cout << "Function returned: " << result << std::endl;
            }
        }
    }
}

void InputManager::PollEvents()
{
    glfwPollEvents();
}
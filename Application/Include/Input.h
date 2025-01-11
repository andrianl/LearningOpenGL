#pragma once
#include <unordered_map>
#include <Delegate.h>

struct GLFWwindow;

void processInput(GLFWwindow* window);

int ChangePolygonMode(GLFWwindow* window);

class InputManager
{
public:
    InputManager(GLFWwindow* window_ptr);
    template <typename ReturnType, typename... Args>
    void SubscribeToKey(int key, std::function<ReturnType(Args...)> func);
    void PollEvents();

    static inline InputManager* GetInputManager() { return currentInstance; }

private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    GLFWwindow* window = nullptr;
    static std::unordered_map<int, Delegate> keyDelegates;
    static InputManager* currentInstance;
};  

template <typename ReturnType, typename... Args>
inline void InputManager::SubscribeToKey(int key, std::function<ReturnType(Args...)> func)
{
    keyDelegates[key].AddDynamic(func);
}
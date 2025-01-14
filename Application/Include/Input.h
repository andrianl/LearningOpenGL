#pragma once
#include <unordered_map>
#include <Delegate.h>

struct GLFWwindow;

void processInput(GLFWwindow* window);

int ChangePolygonMode(GLFWwindow* window);

class InputManager
{
public:
    // Constructor
    // This constructor initializes the InputManager with a GLFW window pointer and sets up the key callback function.
    //
    // Parameters:
    // - window_ptr: A pointer to the GLFW window.
    explicit InputManager(GLFWwindow* window_ptr);

    // Subscribe to key events
    // This method allows subscribing to key events by specifying a key and a function to be called when the key is pressed.
    //
    // Parameters:
    // - key: The key code to subscribe to.
    // - func: The function to be called when the key is pressed.
    template <typename ReturnType, typename... Args>
    void SubscribeToKey(int key, std::function<ReturnType(Args...)> func);

    // Poll for events
    // This method polls for and processes events, such as key presses, for the GLFW window.
    void PollEvents();

    // Get the current instance of InputManager
    // This inline static method returns a pointer to the current instance of InputManager.
    //
    // Returns:
    // - A pointer to the current instance of InputManager.
    static inline InputManager* GetInputManager() { return currentInstance; }

private:
    // Key callback function
    // This static method is called whenever a key is pressed, released, or repeated in the GLFW window. It checks if there are any
    // subscribed functions for the key and calls them.
    //
    // Parameters:
    // - window: A pointer to the GLFW window.
    // - key: The key code of the key that was pressed, released, or repeated.
    // - scancode: The platform-specific scancode of the key.
    // - action: The action (press, release, or repeat) for the key.
    // - mods: Bit field describing which modifier keys were held down.
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    GLFWwindow* window = nullptr;                           // Pointer to the GLFW window
    static std::unordered_map<int, Delegate> keyDelegates;  // Map of key codes to delegate functions
    static InputManager* currentInstance;                   // Pointer to the current instance of InputManager
};

template <typename ReturnType, typename... Args>
inline void InputManager::SubscribeToKey(int key, std::function<ReturnType(Args...)> func)
{
    keyDelegates[key].AddDynamic(func);
}

#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>
#include <any>

// Universal template class Delegate
class Delegate
{
public:
    // Add a function to the delegate with perfect forwarding
    template <typename ReturnType, typename... Args>
    void AddDynamic(std::function<ReturnType(Args...)> func)
    {
        // Get the type index of the function type
        std::type_index type = std::type_index(typeid(std::function<ReturnType(Args...)>));

        // Add the function to the vector of listeners for this type
        listeners[type].push_back(std::any(func));
    }

    // Broadcast the function call to all listeners and return their results
    template <typename ReturnType, typename... Args>
    std::vector<ReturnType> Broadcast(Args... args)
    {
        // Get the type index of the function type
        std::type_index type = std::type_index(typeid(std::function<ReturnType(Args...)>));

        // Container to store results of function calls
        std::vector<ReturnType> results;

        // Loop through all listeners and call their functions
        for (auto& listener : listeners[type])
        {
            // Cast the listener to the correct function type and call it
            auto func = std::any_cast<std::function<ReturnType(Args...)>>(listener);
            results.push_back(func(args...));
        }

        // Return the results of all function calls
        return results;
    }

private:
    // Map to store listeners based on their function type
    std::unordered_map<std::type_index, std::vector<std::any>> listeners;
};

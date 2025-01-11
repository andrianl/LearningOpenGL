#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>
#include <any>

// Шаблонний клас Delegate
class Delegate
{
public:
    template <typename ReturnType, typename... Args>
    void AddDynamic(std::function<ReturnType(Args...)> func)
    {
        std::type_index type = std::type_index(typeid(std::function<ReturnType(Args...)>));
        listeners[type].push_back(std::any(func));
    }
    template <typename ReturnType, typename... Args>
    std::vector<ReturnType> Broadcast(Args... args)
    {
        std::type_index type = std::type_index(typeid(std::function<ReturnType(Args...)>));
        std::vector<ReturnType> results;
        for (auto& listener : listeners[type])
        {
            auto func = std::any_cast<std::function<ReturnType(Args...)>>(listener);
            results.push_back(func(args...));
        }
        return results;
    }

private:
    std::unordered_map<std::type_index, std::vector<std::any>> listeners;
};
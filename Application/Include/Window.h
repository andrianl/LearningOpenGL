#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void OpenGLVersionInit(int Major, int Minor);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Process keyboard input
void processInput(GLFWwindow* window);
#define STB_IMAGE_IMPLEMENTATION
#include "Core.h"     // Contains OpenGL version initialization, MemoryInfo, etc.
#include "Window.h"   // Custom window wrapper class
#include "Input.h"    // InputManager class
#include "VAO.h"      // VertexArrayObject & GlobalVAO classes
#include "VBO.h"      // VertexBufferObject class
#include "EBO.h"      // ElementBufferObject class
#include "Shaders.h"  // Shader class
#include "Texture.h"  // Texture class
#include "Vertex.h"   // Vertex and VertexLayout definitions

#include <array>
#include <iostream>

int main(void)
{
    // Retrieve the global memory information structure
    MemoryInfo& memInfo = getMemoryInfo();

    // Initialize GLFW library
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // Initialize OpenGL version (e.g., 4.6)
    OpenGLVersionInit(4, 6);

    // Create a window using the custom Window class
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Learn OpenGL", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW and verify success
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error: GLEW initialization failed!" << std::endl;
        return -2;
    }

    // Set the viewport size and register a framebuffer resize callback
    glViewport(0, 0, 1920, 1080);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Create an InputManager instance for handling input events
    InputManager input(window);

    // Disable vertical synchronization to allow unlimited frame rates
    glfwSwapInterval(0);

    // Print the current OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Define vertex data using a std::array of Vertex2DUV.
    // Each vertex contains a 2D position and a 2D texture coordinate.
    std::array<Vertex2DUV, 4> vertices = {
        Vertex2DUV({glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 0.0f)}),  // Bottom-left
        Vertex2DUV({glm::vec2(0.5f, -0.5f), glm::vec2(1.0f, 0.0f)}),   // Bottom-right
        Vertex2DUV({glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 1.0f)}),    // Top-right
        Vertex2DUV({glm::vec2(-0.5f, 0.5f), glm::vec2(0.0f, 1.0f)})    // Top-left
    };

    // Define index data for two triangles forming a rectangle.
    std::array<GLuint, 6> indices = {0, 1, 2, 2, 3, 0};

    // ----------------------------
    // Setup Vertex Array, Buffer, and Element Buffer
    // ----------------------------

    // Create and bind a Vertex Array Object (VAO)
    VertexArrayObject vao;
    vao.Bind();

    // Create and bind a Vertex Buffer Object (VBO), then upload vertex data
    VertexBufferObject vbo;
    vbo.Bind();
    vbo.UploadData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

    // Instead of manually specifying each attribute, use the template function
    // which uses the VertexLayout specialization for VertexPosUV2D.
    vao.EnableVertexAttributes<VertexPosUV2D>();

    // Create and bind an Element Buffer Object (EBO), then upload index data
    ElementBufferObject ebo;
    ebo.Bind();
    ebo.UploadData(indices.size() * sizeof(GLuint), indices.data());

    // Unbind the VAO (optional, for safety)
    vao.Unbind();

    // ----------------------------
    // Setup Shader and Texture
    // ----------------------------

    // Initialize the shader program from file and bind it for use
    Shader shader("../Application/Resources/Shaders/Test.shader");
    shader.Bind();

    // Initialize and bind the texture
    Texture wallTexture("../Application/Resources/Textures/wall.jpg");
    wallTexture.Bind();

    // ----------------------------
    // Main Render Loop
    // ----------------------------
    while (!glfwWindowShouldClose(window))
    {
        // Process input (keyboard, mouse, etc.)
        processInput(window);
        glfwPollEvents();
        // Clear the screen with the default clear color
        glClear(GL_COLOR_BUFFER_BIT);

        // Update shader uniform "time" with the current time value
        glUniform1f(glGetUniformLocation(shader.GetShaderID(), "time"), static_cast<GLfloat>(glfwGetTime()));

        // Bind the VAO and draw the elements as triangles
        vao.Bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
        vao.Unbind();

        // Swap buffers and poll events
        glfwSwapBuffers();
    }

    // ----------------------------
    // Cleanup and Memory Reporting
    // ----------------------------
    glfwTerminate();

    std::cout << "Total allocated memory: " << memInfo.totalAllocated << " bytes" << std::endl;
    std::cout << "Total freed memory: " << memInfo.totalFreed << " bytes" << std::endl;
    std::cout << "Used memory: " << memInfo.getUsedMemory() << " bytes" << std::endl;
    std::cout << "Memory allocation count: " << memInfo.allocationCount << std::endl;
    std::cout << "Memory deallocation count: " << memInfo.deallocationCount << std::endl;

    return 0;
}

#define STB_IMAGE_IMPLEMENTATION
#include "Core.h"
#include "Shaders.h"
#include "Texture.h"
#include "Input.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Window.h"
#include <Vertex.h>

int main(void)
{

    MemoryInfo& memInfo = getMemoryInfo();
    GLFWwindow* window;
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
        GLFWwindow* window = glfwCreateWindow(1920, 1080, "Learn OpenGL", NULL, NULL);
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

            // Swap front and back buffers (double-buffering)
            glfwSwapBuffers(window);
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

    // Initialize the GLFW library
    if (!glfwInit()) return -1;  // Return if initialization failed

    OpenGLVersionInit(4, 6);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(1920, 1080, "Learn OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();  // Terminate GLFW if window creation failed
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW and check for errors
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error: GLEW initialization failed!" << std::endl;
        return -2;
    }

    glViewport(0, 0, 1920, 1080);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    InputManager IM(window);

    // Disable vertical sync (0 to avoid limiting frame rate)
    glfwSwapInterval(0);

    // Print the version of OpenGL in use
    std::cout << glGetString(GL_VERSION) << std::endl;

    // Vertex positions (2D) and texture coordinates (interleaved)
    // Створюємо масив вершин за допомогою std::array
    std::array<Vertex2DUV, 4> vertices = {
        Vertex2DUV({glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 0.0f)}),  // Vertex 0 (bottom-left)
        Vertex2DUV({glm::vec2(0.5f, -0.5f), glm::vec2(1.0f, 0.0f)}),   // Vertex 1 (bottom-right)
        Vertex2DUV({glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 1.0f)}),    // Vertex 2 (top-right)
        Vertex2DUV({glm::vec2(-0.5f, 0.5f), glm::vec2(0.0f, 1.0f)})    // Vertex 3 (top-left)
    };

    std::array<GLuint, 6> indices = {
        0, 1, 2,  // First Triangle
        2, 3, 0   // Second Triangle
    };
    

    // Initialize VAO
    GlobalVAO& GVAO = GlobalVAO::GetInstance();
    VertexArrayObject vao;
    vao.Bind();

    // Initialize and bind VBO
    VertexBufferObject vbo;
    vbo.Bind();

    vbo.UploadData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.data()[0]), vertices.data(), GL_STATIC_DRAW);

    // Position attribute (2D positions)
    vao.EnableAttribute(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);  // 2D position

    // Texture coordinate attribute
    vao.EnableAttribute(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));  // Texture coordinates

    // Initialize and bind EBO
    ElementBufferObject ebo;
    ebo.Bind();
    ebo.UploadData(indices.size() * sizeof(indices.data()[0]), indices.data());

    // Initialize Shader
    //Shader Gradient("Application/Resources/Shaders/Test.shader");

    Shader Gradient("../Application/Resources/Shaders/Test.shader");

    Gradient.Bind();

    // Initialize Texture
    Texture WallTexture("../Application/Resources/Textures/wall.jpg");
    WallTexture.Bind();

    // Main render loop: continues until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(Gradient.GetShaderID(), "time"), static_cast<GLfloat>(glfwGetTime()));

        // Render the elements using indices, defined as triangles
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        // Swap front and back buffers (double-buffering)
        glfwSwapBuffers(window);

        // Poll for and process events (keyboard, mouse, etc.)
        IM.PollEvents();
    }

    // Terminate GLFW and clean up
    glfwTerminate();
    std::cout << "Total allocated memory: " << memInfo.totalAllocated << " bytes" << std::endl;
    std::cout << "Total freed memory: " << memInfo.totalFreed << " bytes" << std::endl;
    std::cout << "Used memory: " << memInfo.getUsedMemory() << " bytes" << std::endl;
    std::cout << "Memory allocation count: " << memInfo.allocationCount << std::endl;
    std::cout << "Memory deallocation count: " << memInfo.deallocationCount << std::endl;

    return 0;
}

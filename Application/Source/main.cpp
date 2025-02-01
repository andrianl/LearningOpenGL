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
#include "SSE.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(void)
{
    float StartProgramTime = glfwGetTime();

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // Initialize OpenGL version (e.g., 4.6)
    OpenGLVersionInit(4, 6);

    glfwWindowHint(GLFW_SAMPLES, 16); // 4x MSAA
    // Create a window using the custom Window class
    GLFWwindow* window = glfwCreateWindow(3840, 2160, "Learn OpenGL", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error: GLEW initialization failed!" << std::endl;
        return -2;
    }
    glEnable(GL_MULTISAMPLE);

    // Set the viewport size and register a framebuffer resize callback
    glViewport(0, 0, 3840, 2160);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Create an InputManager instance
    InputManager input(window);

    // Disable vertical synchronization
    glfwSwapInterval(0);

    // Print OpenGL version and SSE level info
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    SSELevel sseLevel = GetSupportedSSELevel();
    std::cout << "Supported SSE Level: " << SSELevelToString(sseLevel) << std::endl;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // ------------------------------------------------------------------------
    // Vertex data for a cube (36 vertices, each with 5 floats: position (3) + texture coord (2))
    // ------------------------------------------------------------------------

// Вершини з UV координатами в одному масиві
    std::array<float, 40> vertices = {
        // positions         // texture coords (UV)
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  // top-left
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // bottom-right

         // Back face
          0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  // back bottom-right
          0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  // back top-right
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // back top-left
         -0.5f, -0.5f, -0.5f,  1.0f, 0.0f   // back bottom-left
    };

    // Індекси в одному масиві
    std::array<uint32_t, 36> indices = {
        0, 1, 2,  2, 3, 0,  // Front
        3, 2, 5,  5, 4, 3,  // Right
        4, 5, 6,  6, 7, 4,  // Back
        7, 6, 1,  1, 0, 7,  // Left
        1, 6, 5,  5, 2, 1,  // Top
        7, 0, 3,  3, 4, 7   // Bottom
    };




    // ------------------------------------------------------------------------
    // World space positions for the 10 cubes
    // ------------------------------------------------------------------------
    std::array<glm::vec3, 10> cubePositions = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // ------------------------------------------------------------------------
    // Setup Vertex Array Object (VAO), Vertex Buffer Object (VBO) and Element Buffer Object (EBO)
    // ------------------------------------------------------------------------
    VertexArrayObject vao;
    vao.Bind();

    VertexBufferObject vbo;
    vbo.Bind();
    vbo.UploadData(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

    // Enable vertex attributes using the layout defined in VertexPosUV2D.
    vao.EnableVertexAttributes<VertexPosUV3D>();

    ElementBufferObject ebo;
    ebo.Bind();
    ebo.UploadData(indices.size() * sizeof(GLuint), indices.data());

    vao.Unbind();

    // ------------------------------------------------------------------------
    // Setup Shader and Texture
    // ------------------------------------------------------------------------
    Shader shader("../Application/Resources/Shaders/TestTexture.shader");
    Texture wallTexture("../Application/Resources/Textures/awesomeface.png");

    // Create view and projection matrices
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 3840.f / 2160.f, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    shader.Bind();
    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);

    // Bind the texture to texture unit 0 and update the shader uniform
    wallTexture.BindTextureToShader(0, shader, "ourTexture");

    // ------------------------------------------------------------------------
    // Main Render Loop
    // ------------------------------------------------------------------------
    glm::vec4 ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        float StartOfFrameTime = glfwGetTime();
        processInput(window);
        glfwPollEvents();

        glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vao.Bind();

        // Для кожного куба встановлюємо модельну матрицю і малюємо куб через glDrawElements
        for (const auto& cube : cubePositions)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube);
            float angle = 10.0f + ((std::sin(glfwGetTime()) + 1.0f) / 2.0f) * (90.0f - 10.0f);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.SetMat4("model", model);

            // Малюємо куб з 36 індексів (GL_TRIANGLES)
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        vao.Unbind();
        glfwSwapBuffers(window);
        float EndOfFrameTime = glfwGetTime();
    }

    glfwTerminate();
    return 0;
}

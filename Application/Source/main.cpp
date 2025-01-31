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

#include <GL/glew.h>     // Include GLEW to manage OpenGL extensions
#include <GLFW/glfw3.h>  // Include GLFW for window creation and management
#include <iostream>      // Include for standard I/O operations
#include <Platform.h>    // Platform-specific includes (this is custom, probably for cross-platform support)
#include "Shaders.h"     // Custom header for shader-related functions
#include <Core.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit()) return -1;  // Return if initialization failed

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 720, "Learn OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();  // Terminate GLFW if window creation failed
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize GLEW and check for errors */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error: GLEW is not OK" << std::endl;
        return -2;
    }

    // Disable vertical sync (0 to avoid limiting frame rate)
    glfwSwapInterval(0);

    // Print the version of OpenGL in use
    std::cout << glGetString(GL_VERSION) << std::endl;

    // Vertex positions for a square (two triangles)
    const float pos[] = {
        -0.5f, -0.5f,  // Vertex 0 (bottom-left)
         0.5f, -0.5f,   // Vertex 1 (bottom-right)
         0.5f,  0.5f,    // Vertex 2 (top-right)
        -0.5f,  0.5f    // Vertex 3 (top-left)
    };

    // Indices defining two triangles using the vertices
    const uint32 indices[] = {
        0, 1, 2,  // Indices for first triangle
        2, 3, 0   // Indices for second triangle
    };

    // Size of the vertex positions array
    constexpr auto size_of_pos = sizeof(pos);
    // Size of the indices array
    constexpr auto size_of_indicies = sizeof(indices);
    // Total number of indices in the array
    constexpr auto number_of_indicies = sizeof(indices) / sizeof(indices[0]);
    // Total number of triangles
    //constexpr auto number_of_tries = 2;
    // Number of vertices per triangle
    //constexpr auto number_of_vertex = size_of_pos / sizeof(float) / number_of_tries;

    // Generate and bind a buffer for vertex data (VBO)
    uint32 buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size_of_pos, pos, GL_STATIC_DRAW);

    // Enable the vertex attribute array and define how the vertex data is laid out
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    // Generate and bind a buffer for element indices (EBO or IBO)
    uint32 index_buffer_object;
    glGenBuffers(1, &index_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_of_indicies, indices, GL_STATIC_DRAW);

    // Parse the shader from a file
    const ShaderSourceProgram Source = ParseShader("Application/Resources/Shaders/Test.shader");

    // Create a shader program from vertex and pixel shaders
    const uint32 Shader = CreateShader(Source.VertexShader, Source.PixelShader);
    glUseProgram(Shader);  // Use the created shader program

    /* Main render loop: this loop continues until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear the screen */
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(Shader, "time"), glfwGetTime());

        /* Render the elements using indices, defined as triangles */
        glDrawElements(GL_TRIANGLES, number_of_indicies, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers (double-buffering) */
        glfwSwapBuffers(window);

        /* Poll for and process events (keyboard, mouse, etc.) */
        glfwPollEvents();
    }

    /* Terminate GLFW and clean up */
    glfwTerminate();
    MemoryInfo memInfo = getMemoryInfo();
    std::cout << "Total allocated memory: " << memInfo.totalAllocated << " bytes" << std::endl;
    std::cout << "Total freed memory: " << memInfo.totalFreed << " bytes" << std::endl;
    std::cout << "Used memory: " << memInfo.getUsedMemory() << " bytes" << std::endl;
    std::cout << "Memory allocation count: " << memInfo.allocationCount << std::endl;
    std::cout << "Memory deallocation count: " << memInfo.deallocationCount << std::endl;

    return 0;
}
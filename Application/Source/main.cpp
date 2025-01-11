#define STB_IMAGE_IMPLEMENTATION
#include <Core.h>

int main(void)
{
    MemoryInfo& memInfo = getMemoryInfo();
    GLFWwindow* window;

    // Initialize the GLFW library
    if (!glfwInit()) return -1;  // Return if initialization failed

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
    const float vertices[] = {
        // Position           // Texture Coordinates
        -0.5f, -0.5f, 0.0f, 0.0f,  // Vertex 0 (bottom-left)
        0.5f, -0.5f, 1.0f, 0.0f,   // Vertex 1 (bottom-right)
        0.5f, 0.5f, 1.0f, 1.0f,    // Vertex 2 (top-right)
        -0.5f, 0.5f, 0.0f, 1.0f    // Vertex 3 (top-left)
    };

    // Indices defining two triangles using the vertices
    const uint32 indices[] = {
        0, 1, 2,  // Indices for first triangle
        2, 3, 0   // Indices for second triangle
    };


    uint32 VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Calculate the size of the vertex positions array
    constexpr auto size_of_pos = sizeof(vertices);
    // Calculate the size of the indices array
    constexpr auto size_of_indices = sizeof(indices);
    // Calculate the total number of indices in the array
    constexpr auto number_of_indices = sizeof(indices) / sizeof(indices[0]);

    // Generate and bind a buffer for vertex data (VBO)
    uint32 buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, size_of_pos, vertices, GL_STATIC_DRAW);

    // Position attribute (2D positions)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);  // 2D position
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));  // Texture coordinates
    glEnableVertexAttribArray(1);

    // Generate and bind a buffer for element indices (EBO or IBO)
    uint32 index_buffer_object;
    glGenBuffers(1, &index_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_of_indices, indices, GL_STATIC_DRAW);

    const Shader Gradient("Application/Resources/Shaders/TestTexture.shader");

    Gradient.Use();

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Application/Resources/Textures/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }

    InputManager::GetInputManager()->SubscribeToKey<int, GLFWwindow*>(GLFW_KEY_F, ChangePolygonMode);
    //Main render loop: continues until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(Gradient.GetShaderID(), "time"), static_cast<GLfloat>(glfwGetTime()));

        // Render the elements using indices, defined as triangles
        glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, nullptr);

        // Swap front and back buffers (double-buffering)
        glfwSwapBuffers(window);

        // Poll for and process events (keyboard, mouse, etc.)
        //glfwPollEvents();

        IM.PollEvents();
    }

    // Terminate GLFW and clean up
    glfwTerminate();
    stbi_image_free(data);
    std::cout << "Total allocated memory: " << memInfo.totalAllocated << " bytes" << std::endl;
    std::cout << "Total freed memory: " << memInfo.totalFreed << " bytes" << std::endl;
    std::cout << "Used memory: " << memInfo.getUsedMemory() << " bytes" << std::endl;
    std::cout << "Memory allocation count: " << memInfo.allocationCount << std::endl;
    std::cout << "Memory deallocation count: " << memInfo.deallocationCount << std::endl;

    return 0;
}
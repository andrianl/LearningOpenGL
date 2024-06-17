#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Platform.h>
#include "Shaders.h"

std::string VS =
"#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"    TexCoord = position.xy * 0.5 + 0.5;\n"
"    gl_Position = vec4(position, 1.0);\n"
"}\n";

std::string PS =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"void main()\n"
"{\n"
"    vec3 gradient_color = vec3(1.0, 1.0, 0.5) * (1.0 - TexCoord.y) + vec3(1.0, 0.0, 1.0) * TexCoord.y;\n"
"    FragColor = vec4(gradient_color, 1.0);\n"
"}\n";

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error: GLEW is not OK" << std::endl;
		return -2;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float pos[6] = { -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f };

	uint32 buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	uint32 Shader = CreateShader(VS, PS);
	glUseProgram(Shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		glDrawArrays(GL_TRIANGLES, 0, 3);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
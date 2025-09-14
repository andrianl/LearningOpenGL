#include "Window.h"  // Custom window wrapper class
#include "VAO.h"     // VertexArrayObject & GlobalVAO classes
#include "VBO.h"     // VertexBufferObject class
#include "EBO.h"     // ElementBufferObject class
#include "Shaders.h" // Shader class
#include "Texture.h" // Texture class
#include "Vertex.h"  // Vertex and VertexLayout definitions
#include "Camera.h" // Camera class

#include <array>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Cube.h>

#include <random>
#include <Maths.h>

struct MaterialData {
	glm::vec3 ambient;   // 0..1
	glm::vec3 diffuse;   // 0..1
	glm::vec3 specular;  // 0..1
	float shininess;     // 0..1 (якщо хочеш нормалізувати)
};

static glm::vec3 generateAxisFromIndex(size_t index)
{
	unsigned int seed = static_cast<unsigned int>(index * 73856093u ^ 0x9e3779b9u);
	auto rand01 = [&seed]() {
		seed ^= seed << 13;
		seed ^= seed >> 17;
		seed ^= seed << 5;
		return (seed & 0xFFFFFF) / static_cast<float>(0xFFFFFF);
		};

	glm::vec3 axis(rand01() * 2.0f - 1.0f,
		rand01() * 2.0f - 1.0f,
		rand01() * 2.0f - 1.0f);

	if (glm::length(axis) < 0.001f)
		axis = glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::normalize(axis);
}


// Global variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool keys[1024];

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.processMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(static_cast<float>(yoffset));
}

// Key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	// Camera mode switching
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_1:
			camera.setPortraitMode();
			std::cout << "Portrait Mode: 85mm, f/1.8" << std::endl;
			break;
		case GLFW_KEY_2:
			camera.setLandscapeMode();
			std::cout << "Landscape Mode: 24mm, f/8" << std::endl;
			break;
		case GLFW_KEY_3:
			camera.setSportsMode();
			std::cout << "Sports Mode: 200mm, f/2.8" << std::endl;
			break;
		case GLFW_KEY_4:
			camera.setNightMode();
			std::cout << "Night Mode: 50mm, f/1.4, ISO 1600" << std::endl;
			break;
		}
	}
}

// Process keyboard input
void processInput(GLFWwindow* window)
{
	glm::vec3 moveDir(0.0f);

	if (keys[GLFW_KEY_W])
		moveDir += glm::vec3(0.0f, 0.0f, 1.0f);
	if (keys[GLFW_KEY_S])
		moveDir += glm::vec3(0.0f, 0.0f, -1.0f);
	if (keys[GLFW_KEY_A])
		moveDir += glm::vec3(-1.0f, 0.0f, 0.0f);
	if (keys[GLFW_KEY_D])
		moveDir += glm::vec3(1.0f, 0.0f, 0.0f);
	if (keys[GLFW_KEY_SPACE])
		moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
	if (keys[GLFW_KEY_LEFT_SHIFT])
		moveDir += glm::vec3(0.0f, -1.0f, 0.0f);

	if (glm::length(moveDir) > 0.0f)
		camera.processMovement(glm::normalize(moveDir), deltaTime);

	// Camera settings adjustment
	if (keys[GLFW_KEY_Q])
	{
		float currentAperture = camera.getAperture();
		camera.setAperture(currentAperture - 1.0f * deltaTime);
	}
	if (keys[GLFW_KEY_E])
	{
		float currentAperture = camera.getAperture();
		camera.setAperture(currentAperture + 1.0f * deltaTime);
	}

	if (keys[GLFW_KEY_R])
	{
		int currentISO = camera.getISO();
		camera.setISO(static_cast<int>(currentISO * (1.0f + 0.5f * deltaTime)));
	}
	if (keys[GLFW_KEY_F])
	{
		int currentISO = camera.getISO();
		camera.setISO(static_cast<int>(currentISO * (1.0f - 0.5f * deltaTime)));
	}
}

int main()
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

    //glfwWindowHint(GLFW_SAMPLES, 4);

	constexpr bool fullscreen = true;
	GLFWwindow* window;
	uint32  modeWidth = 1920;
	uint32  modeHeight = 1080;
	if (!fullscreen)
	{
		window = glfwCreateWindow(modeWidth, modeHeight, "Learn OpenGL", nullptr, nullptr);
	}
	else
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		window = glfwCreateWindow(
			mode->width, mode->height,
			"Learn OpenGL",
			monitor,
			nullptr
		);

		modeWidth = static_cast<uint32>(mode->width);
		modeHeight = static_cast<uint32>(mode->height);
	}

    // Create a window using the custom Window class
   //GLFWwindow *window = glfwCreateWindow(1920, 1080, "Learn OpenGL", nullptr, nullptr);
   
	if (!window)
    {
        glfwTerminate();
        return -1;
    }

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Make the window's context current
    glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error: GLEW initialization failed!" << std::endl;
        return -2;
    }

    glEnable(GL_MULTISAMPLE);

    // Set the viewport size and register a framebuffer resize callback
    glViewport(0, 0, modeWidth, modeHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Disable vertical synchronization
    glfwSwapInterval(0);

    // Print OpenGL version and SSE level info
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


    GraphicsShader shader("../Application/Resources/Shaders/TestLight.shader");
    //Texture wallTexture("../Application/Resources/Textures/awesomeface.png");

	Cube cubeMesh;

	camera.setMovementSpeed(5.0f);
	camera.setMouseSensitivity(0.1f);
    // Create view and projection matrices
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(modeWidth) / static_cast<float>(modeHeight), 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    shader.Bind();
    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);

    // Bind the texture to texture unit 0 and update the shader uniform
    //wallTexture.BindTextureToShader(0, shader, "ourTexture");

    // ------------------------------------------------------------------------
    // Main Render Loop
    // ------------------------------------------------------------------------
	glm::vec3 skyColor = glm::vec3(0.53f, 0.81f, 0.92f);

	// Light uniforms

	glm::vec3 lightColor = glm::vec3(1.0f);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5);
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.1f);
	glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

	shader.SetVec3("light.position", glm::vec3(5.0f, 5.0f, 5.0f));
	shader.SetVec3("light.diffuse", diffuseColor);
	shader.SetVec3("light.ambient", ambientColor);
	shader.SetVec3("light.specular", specularColor);


	size_t numCubes = 10000;
	std::vector<Cube> cubes(numCubes);


	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(-100.0f, 100.0f);

	for (auto& cube : cubes)
	{
		// Material uniforms

		glm::vec3 pos(dist(rng), dist(rng), dist(rng));
		cube.position = pos;
	}
	std::vector<MaterialData> materials(numCubes);

	std::mt19937 mrng(std::random_device{}());
	std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

	for (size_t j = 0; j < numCubes; ++j) {
		materials[j].ambient = glm::vec3(dist01(mrng), dist01(mrng), dist01(mrng));
		materials[j].diffuse = glm::vec3(dist01(mrng), dist01(mrng), dist01(mrng));
		materials[j].specular = glm::vec3(dist01(mrng), dist01(mrng), dist01(mrng));
		materials[j].shininess = dist01(mrng);
	}

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glfwPollEvents();

		glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Camera uniforms
		shader.SetMat4("view", camera.getViewMatrix());

		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),
			static_cast<float>(modeWidth) / static_cast<float>(modeHeight),
			0.1f,
			100.0f
		);

		shader.SetMat4("projection", projection);
		shader.SetVec3("viewPos", camera.getPosition());

		size_t i = 0;
		for (auto& cube : cubes)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cube.position);

			glm::vec3 axis = generateAxisFromIndex(i);
			float angle = glfwGetTime() * (20.0f + (i % 5) * 10.0f);
			model = glm::rotate(model, glm::radians(angle), axis);

			// === Перевірка видимості ===
			glm::vec4 clipPos = projection * camera.getViewMatrix() * model * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			// Якщо позаду камери
			if (clipPos.w <= 0.0f) { i++; continue; }

			glm::vec3 ndc = glm::vec3(clipPos) / clipPos.w;

			// Якщо центр куба поза екраном
			if (ndc.x < -1.0f || ndc.x > 1.0f ||
				ndc.y < -1.0f || ndc.y > 1.0f ||
				ndc.z < 0.0f || ndc.z > 1.0f)
			{
				i++;
				continue;
			}

			// Якщо пройшов перевірку — рендеримо
			shader.SetMat4("model", model);
			shader.SetVec3("material.ambient", materials[i].ambient);
			shader.SetVec3("material.diffuse", materials[i].diffuse);
			shader.SetVec3("material.specular", materials[i].specular);
			shader.SetFloat("material.shininess", materials[i].shininess * 128.0f);

			cubeMesh.Draw();

			i++;
		}

		glfwSwapBuffers(window);
	}

    glfwTerminate();
    return 0;
}

#include "Window.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shaders.h"
#include "Texture.h"
#include "Vertex.h"
#include "Camera.h"
#include "Light.h"
#include "Cube.h"
#include "Maths.h"

#include <array>
#include <iostream>
#include <vector>
#include <random>
#include <execution>
#include <future>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct DrawCommand {
	glm::mat4 model;
	size_t materialIndex;
	bool visible;
};


// Material structure
struct MaterialData {
	glm::vec3 Color;
	float Intensity;
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shininess;
};

// Global variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool keys[1024] = { false };

// Light constants
constexpr int NUM_DIRECTIONAL = 1;
constexpr int NUM_POINT = 10;
constexpr int NUM_SPOT = 10;
constexpr size_t NUM_CUBES = 10000;
constexpr float WORLD_SIZE = 100.0f;

std::vector<DirectionalLight> dirLights(NUM_DIRECTIONAL);
std::vector<PointLight> pointLights(NUM_POINT);
std::vector<SpotLight> spotLights(NUM_SPOT);

// Generate random rotation axis for cube based on its index
static glm::vec3 generateAxisFromIndex(size_t index) {
	unsigned int seed = static_cast<unsigned int>(index * 73856093u ^ 0x9e3779b9u);

	auto rand01 = [&seed]() {
		seed ^= seed << 13;
		seed ^= seed >> 17;
		seed ^= seed << 5;
		return (seed & 0xFFFFFF) / static_cast<float>(0xFFFFFF);
		};

	glm::vec3 axis(
		rand01() * 2.0f - 1.0f,
		rand01() * 2.0f - 1.0f,
		rand01() * 2.0f - 1.0f
	);

	if (glm::length(axis) < 0.001f) {
		axis = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	return glm::normalize(axis);
}

// Frustum culling check
bool isInFrustum(const glm::vec3& position, const glm::mat4& viewProjection) {
	glm::vec4 clipPos = viewProjection * glm::vec4(position, 1.0f);

	// Behind camera
	if (clipPos.w <= 0.0f) return false;

	glm::vec3 ndc = glm::vec3(clipPos) / clipPos.w;

	// Outside screen bounds
	return !(ndc.x < -1.0f || ndc.x > 1.0f ||
		ndc.y < -1.0f || ndc.y > 1.0f ||
		ndc.z < 0.0f || ndc.z > 1.0f);
}

// Input callbacks
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.processMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processMouseScroll(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}

	// Camera mode switching
	if (action == GLFW_PRESS) {
		switch (key) {
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

void processInput(GLFWwindow* window) {
	glm::vec3 moveDir(0.0f);

	// Movement input
	if (keys[GLFW_KEY_W]) moveDir += glm::vec3(0.0f, 0.0f, 1.0f);
	if (keys[GLFW_KEY_S]) moveDir += glm::vec3(0.0f, 0.0f, -1.0f);
	if (keys[GLFW_KEY_A]) moveDir += glm::vec3(-1.0f, 0.0f, 0.0f);
	if (keys[GLFW_KEY_D]) moveDir += glm::vec3(1.0f, 0.0f, 0.0f);
	if (keys[GLFW_KEY_SPACE]) moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
	if (keys[GLFW_KEY_LEFT_SHIFT]) moveDir += glm::vec3(0.0f, -1.0f, 0.0f);

	if (glm::length(moveDir) > 0.0f) {
		camera.processMovement(glm::normalize(moveDir), deltaTime);
	}

	// Camera settings adjustment
	if (keys[GLFW_KEY_Q]) {
		float currentAperture = camera.getAperture();
		camera.setAperture(currentAperture - 1.0f * deltaTime);
	}
	if (keys[GLFW_KEY_E]) {
		float currentAperture = camera.getAperture();
		camera.setAperture(currentAperture + 1.0f * deltaTime);
	}
	if (keys[GLFW_KEY_R]) {
		int currentISO = camera.getISO();
		camera.setISO(static_cast<int>(currentISO * (1.0f + 0.5f * deltaTime)));
	}
	if (keys[GLFW_KEY_F]) {
		int currentISO = camera.getISO();
		camera.setISO(static_cast<int>(currentISO * (1.0f - 0.5f * deltaTime)));
	}
}

void setupLights(GraphicsShader& shader) 
{
	shader.SetInt("NumDirectionalLights", NUM_DIRECTIONAL);
	shader.SetInt("NumPointLights", NUM_POINT);
	shader.SetInt("NumSpotLights", NUM_SPOT);

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> distDir(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distPos(-50.0f, 50.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);

	// Setup directional lights
	if (NUM_DIRECTIONAL > 0)
	{
		for (int i = 0; i < NUM_DIRECTIONAL; ++i) {
			dirLights[i].SetDirection(glm::normalize(glm::vec3(distDir(rng), distDir(rng), distDir(rng))));
			dirLights[i].SetColor(glm::vec3(distColor(rng), distColor(rng), distColor(rng)));
			dirLights[i].SetIntensity(0.3f); // Reduced intensity to prevent overexposure
			dirLights[i].Apply(&shader, i);
		}
	}

	// Setup point lights
	if (NUM_POINT > 0)
	{
		for (int i = 0; i < NUM_POINT; ++i) {
			pointLights[i].SetPosition(glm::vec3(distPos(rng), distPos(rng), distPos(rng)));
			pointLights[i].SetColor(glm::vec3(distColor(rng), distColor(rng), distColor(rng)));
			pointLights[i].SetIntensity(0.5f); // Reduced intensity
			pointLights[i].SetAttenuation(1.0f, 0.09f, 0.032f);
			pointLights[i].Apply(&shader, i);
		}
	}

	// Setup spot lights

	if (NUM_SPOT > 0)
	{
		for (int i = 0; i < NUM_SPOT; ++i) {
			spotLights[i].SetPosition(glm::vec3(distPos(rng), distPos(rng), distPos(rng)));
			spotLights[i].SetDirection(glm::normalize(glm::vec3(distDir(rng), distDir(rng), distDir(rng))));
			spotLights[i].SetColor(glm::vec3(distColor(rng), distColor(rng), distColor(rng)));
			spotLights[i].SetIntensity(0.5f); // Reduced intensity
			spotLights[i].SetAttenuation(1.0f, 0.09f, 0.032f);
			spotLights[i].SetCutOff(12.5f);
			spotLights[i].SetOuterCutOff(17.5f);
			spotLights[i].Apply(&shader, i);
		}
	}
}

int main() {
	float startProgramTime = glfwGetTime();

	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return -1;
	}

	// Initialize OpenGL version
	OpenGLVersionInit(4, 6);

	// Window setup
	constexpr bool fullscreen = true;
	GLFWwindow* window;
	uint32_t modeWidth = 1920;
	uint32_t modeHeight = 1080;

	if (!fullscreen) {
		window = glfwCreateWindow(modeWidth, modeHeight, "Learn OpenGL", nullptr, nullptr);
	}
	else {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		window = glfwCreateWindow(mode->width, mode->height, "Learn OpenGL", monitor, nullptr);

		modeWidth = static_cast<uint32_t>(mode->width);
		modeHeight = static_cast<uint32_t>(mode->height);
	}

	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Setup window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "Error: GLEW initialization failed!" << std::endl;
		return -2;
	}

	// OpenGL setup
	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0, modeWidth, modeHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(0); // Disable vsync

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	// Enable depth testing and face culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Initialize shader and mesh
	GraphicsShader shader("../Application/Resources/Shaders/TestLight.shader");
	Cube cubeMesh;

	// Setup camera
	camera.setMovementSpeed(5.0f);
	camera.setMouseSensitivity(0.1f);

	// Setup initial matrices
	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		static_cast<float>(modeWidth) / static_cast<float>(modeHeight),
		0.1f, 100.0f
	);

	shader.Bind();
	shader.SetMat4("projection", projection);

	// Setup lighting
	setupLights(shader);

	// Sky color
	bool BlackSky = true;

	glm::vec3 skyColor = glm::vec3(0.53f, 0.81f, 0.92f) * glm::vec3(!BlackSky);

	// Generate cubes
	std::vector<Cube> cubes(NUM_CUBES);
	std::vector<MaterialData> materials(NUM_CUBES);
	std::vector<DrawCommand> drawCommands(cubes.size());


	// Random number generators
	std::mt19937 posRng(std::random_device{}());
	std::mt19937 matRng(std::random_device{}());
	std::uniform_real_distribution<float> posDist(-WORLD_SIZE, WORLD_SIZE);
	std::uniform_real_distribution<float> colorDist(0.1f, 0.9f); // Avoid pure black/white

	// Generate cube positions
	for (auto& cube : cubes) {
		glm::vec3 pos(posDist(posRng), posDist(posRng), posDist(posRng));
		cube.position = pos;
	}

	// Generate materials with better values for PBR-like lighting
	for (size_t i = 0; i < NUM_CUBES; ++i) {
		// Ambient should be quite low since we have proper lighting
		materials[i].Color = glm::vec3(colorDist(matRng), colorDist(matRng), colorDist(matRng));
		materials[i].Intensity = colorDist(matRng) * 0.5f + 0.5f; // 0.5 - 1.0 range for more visible colors
		materials[i].Ambient = glm::vec3(colorDist(matRng), colorDist(matRng), colorDist(matRng)) * 0.1f;
		materials[i].Diffuse = glm::vec3(colorDist(matRng), colorDist(matRng), colorDist(matRng));
		materials[i].Specular = glm::vec3(colorDist(matRng), colorDist(matRng), colorDist(matRng)) * 0.3f;
		materials[i].Shininess = colorDist(matRng) * 96.0f; // 32-128 range for better specular highlights
	}

	// Render loop
	size_t renderedCubes = 0;
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glfwPollEvents();

		glClearColor(skyColor.r, skyColor.g, skyColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 viewProjection = projection * view;

		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);
		shader.SetVec3("ViewPos", camera.getPosition());

		// === Паралельна підготовка команд ===
		std::for_each(std::execution::par, cubes.begin(), cubes.end(),
			[&](const Cube& cube) {
				size_t i = &cube - &cubes[0]; // індекс
				DrawCommand cmd{};
				cmd.materialIndex = i;

				// Фрустум-кулінг
				if (!isInFrustum(cube.position, viewProjection)) {
					cmd.visible = false;
				}
				else {
					cmd.visible = true;
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, cube.position);

					glm::vec3 axis = generateAxisFromIndex(i);
					float angle = glfwGetTime() * (20.0f + (i % 5) * 10.0f);
					model = glm::rotate(model, glm::radians(angle), axis);

					cmd.model = model;
				}
				drawCommands[i] = cmd;
			});

		// === Рендер у головному потоці ===
		renderedCubes = 0;
		pointLights[0].SetPosition(camera.getPosition());
		pointLights[0].SetAttenuation(1.0f, 0.045f, 0.0075f);
		pointLights[0].Apply(&shader, 0);

		for (const auto& cmd : drawCommands) {
			if (!cmd.visible) continue;

			shader.SetMat4("model", cmd.model);

			const MaterialData& material = materials[cmd.materialIndex];
			shader.SetVec3("Material.Color", material.Color);
			shader.SetFloat("Material.Intensity", material.Intensity);
			shader.SetVec3("Material.Ambient", material.Ambient);
			shader.SetVec3("Material.Diffuse", material.Diffuse);
			shader.SetVec3("Material.Specular", material.Specular);
			shader.SetFloat("Material.Shininess", material.Shininess);

			cubeMesh.Draw();
			renderedCubes++;
		}

		glfwSwapBuffers(window);

		static int frameCount = 0;
		if (++frameCount % 60 == 0) {
			std::cout << "Rendered cubes: " << renderedCubes << "/" << NUM_CUBES << std::endl;
		}
	}

	glfwTerminate();
	return 0;
}
#pragma once
#include <array>
#include <Vertex.h>
#include <VAO.h>
#include <VBO.h>
#include <EBO.h>
#include <Shaders.h>

class Cube 
{

public:
	Cube();
	~Cube();


	void Draw();
private:

	void Setup();

public:
	glm::vec3 position{ 0.0f };

private:
	VertexArrayObject VAO;
	VertexBufferObject VBO;
	ElementBufferObject EBO;

	static constexpr float h = 0.5f; // half side length

	static const std::array<VertexPosNormalTangentUV3D, 24> cubeVertices;
	static const std::array<uint32_t, 36> cubeIndices;
};

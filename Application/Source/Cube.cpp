#include "Cube.h"
#include "GL/glew.h"
#include <Maths.h>

const std::array<VertexPosNormalTangentUV3D, 24> Cube::cubeVertices = {
	// +Z
	VertexPosNormalTangentUV3D{{-h,-h, h},{0,0, 1},{ 1,0,0},{0,0}},
	VertexPosNormalTangentUV3D{{ h,-h, h},{0,0, 1},{ 1,0,0},{1,0}},
	VertexPosNormalTangentUV3D{{ h, h, h},{0,0, 1},{ 1,0,0},{1,1}},
	VertexPosNormalTangentUV3D{{-h, h, h},{0,0, 1},{ 1,0,0},{0,1}},

	// -Z
	VertexPosNormalTangentUV3D{{ h,-h,-h},{0,0,-1},{-1,0,0},{0,0}},
	VertexPosNormalTangentUV3D{{-h,-h,-h},{0,0,-1},{-1,0,0},{1,0}},
	VertexPosNormalTangentUV3D{{-h, h,-h},{0,0,-1},{-1,0,0},{1,1}},
	VertexPosNormalTangentUV3D{{ h, h,-h},{0,0,-1},{-1,0,0},{0,1}},

	// -X
	VertexPosNormalTangentUV3D{{-h,-h,-h},{-1,0,0},{0,0, 1},{0,0}},
	VertexPosNormalTangentUV3D{{-h,-h, h},{-1,0,0},{0,0, 1},{1,0}},
	VertexPosNormalTangentUV3D{{-h, h, h},{-1,0,0},{0,0, 1},{1,1}},
	VertexPosNormalTangentUV3D{{-h, h,-h},{-1,0,0},{0,0, 1},{0,1}},

	// +X
	VertexPosNormalTangentUV3D{{ h,-h, h},{1,0,0},{0,0,-1},{0,0}},
	VertexPosNormalTangentUV3D{{ h,-h,-h},{1,0,0},{0,0,-1},{1,0}},
	VertexPosNormalTangentUV3D{{ h, h,-h},{1,0,0},{0,0,-1},{1,1}},
	VertexPosNormalTangentUV3D{{ h, h, h},{1,0,0},{0,0,-1},{0,1}},

	// +Y
	VertexPosNormalTangentUV3D{{-h, h, h},{0,1,0},{ 1,0,0},{0,0}},
	VertexPosNormalTangentUV3D{{ h, h, h},{0,1,0},{ 1,0,0},{1,0}},
	VertexPosNormalTangentUV3D{{ h, h,-h},{0,1,0},{ 1,0,0},{1,1}},
	VertexPosNormalTangentUV3D{{-h, h,-h},{0,1,0},{ 1,0,0},{0,1}},

	// -Y
	VertexPosNormalTangentUV3D{{-h,-h,-h},{0,-1,0},{ 1,0,0},{0,0}},
	VertexPosNormalTangentUV3D{{ h,-h,-h},{0,-1,0},{ 1,0,0},{1,0}},
	VertexPosNormalTangentUV3D{{ h,-h, h},{0,-1,0},{ 1,0,0},{1,1}},
	VertexPosNormalTangentUV3D{{-h,-h, h},{0,-1,0},{ 1,0,0},{0,1}},
};

const std::array<uint32_t, 36> Cube::cubeIndices = {
	0,1,2, 0,2,3,
	4,5,6, 4,6,7,
	8,9,10, 8,10,11,
	12,13,14, 12,14,15,
	16,17,18, 16,18,19,
	20,21,22, 20,22,23
};


Cube::Cube()
{
	Setup();
}

Cube::~Cube()
{
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Cube::Draw()
{
	VAO.Bind();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cubeIndices.size()), GL_UNSIGNED_INT, nullptr);
}

void Cube::Setup()
{
	VAO.Bind();

	VBO.Bind();
	VBO.UploadData(GL_ARRAY_BUFFER, cubeVertices, GL_STATIC_DRAW);

	// Enable vertex attributes using the layout defined in VertexPosUV2D.
	VAO.EnableVertexAttributes<VertexPosNormalTangentUV3D>();

	EBO.Bind();
	EBO.UploadData(cubeIndices.size() * sizeof(GLuint), cubeIndices.data());

	VAO.Unbind();
}


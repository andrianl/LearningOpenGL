#pragma once

#include <vector>
#include <cstddef>  // For offsetof
#include "GL/glew.h"
#include "glm/glm.hpp"

#ifdef _MSC_VER
#include <cstddef>
#define OFFSET_OF(s, m) offsetof(s, m)
#elif defined(__clang__) || defined(CLANG)
#define OFFSET_OF(s, m) (size_t) & reinterpret_cast<const volatile char&>((((s*)0)->m))
#elif defined(_WIN64)
#define OFFSET_OF(s, m) (size_t)((ptrdiff_t) & reinterpret_cast<const volatile char&>((((s*)0)->m)))
#else
#include <cstddef>
#define OFFSET_OF(s, m) offsetof(s, m)
#endif


//==============================================================================
// 1. Template class Vertex
//==============================================================================

template <typename T>
class Vertex
{
public:
    T data;

    Vertex() = default;
    Vertex(const T& data) : data(data) {}

    T& getData() { return data; }
    const T& getData() const { return data; }
};

//==============================================================================
// 2. Structure describing a vertex attribute
//==============================================================================

struct VertexAttrib
{
    GLuint index;          // Shader attribute location
    GLint size;            // Number of components (e.g., 2 for vec2, 3 for vec3)
    GLenum type;           // Data type (e.g., GL_FLOAT, GL_INT, etc.)
    GLsizei stride;        // Total size of the vertex (size of the entire vertex)
    size_t offset;         // Offset of this attribute within the structure
    GLboolean normalized;  // Whether the attribute should be normalized
};

//==============================================================================
// 3. Declaration of vertex structures
//==============================================================================

struct VertexPos2D
{
    glm::vec2 pos;
};

struct VertexPosUV2D
{
    glm::vec2 pos;
    glm::vec2 uv;
};

struct VertexPosUV3D
{
    glm::vec3 pos;
    glm::vec2 uv;
};

struct VertexPosColorUV
{
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 uv;
};

struct VertexPos3D
{
    glm::vec3 pos;
};

struct VertexPosNormal3D
{
    glm::vec3 pos;
    glm::vec3 normal;
};

struct VertexPosNormalUV3D
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct VertexPosColor3D
{
    glm::vec3 pos;
    glm::vec3 color;
};

struct VertexPosColorUV3D
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 uv;
};

struct VertexPosNormalTangentUV3D
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 uv;
};

//==============================================================================
// 4. Using declarations for easier usage of the Vertex template class
//==============================================================================

using Vertex2D = Vertex<VertexPos2D>;
using Vertex2DUV = Vertex<VertexPosUV2D>;
using Vertex2DColorUV = Vertex<VertexPosColorUV>;
using Vertex3D = Vertex<VertexPos3D>;
using Vertex3DNormal = Vertex<VertexPosNormal3D>;
using Vertex3DNormalUV = Vertex<VertexPosNormalUV3D>;
using Vertex3DColor = Vertex<VertexPosColor3D>;
using Vertex3DColorUV = Vertex<VertexPosColorUV3D>;
using Vertex3DNormalTangentUV = Vertex<VertexPosNormalTangentUV3D>;

//==============================================================================
// 5. Template for describing the vertex layout (specializations define GetAttributes)
//==============================================================================

template <typename VertexType>
struct VertexLayout;  // Base template not implemented

// Specialization for VertexPos2D
template <>
struct VertexLayout<VertexPos2D>
{
    static std::vector<VertexAttrib> GetAttributes();
};

// Specialization for VertexPosUV2D
template <>
struct VertexLayout<VertexPosUV2D>
{
    static std::vector<VertexAttrib> GetAttributes();
};

// Specialization for VertexPosUV3D
template <>
struct VertexLayout<VertexPosUV3D>
{
    static std::vector<VertexAttrib> GetAttributes();
};

// Specialization for VertexPosColorUV
template <>
struct VertexLayout<VertexPosColorUV>
{
    static std::vector<VertexAttrib> GetAttributes();
};

// Specialization for VertexPos3D
template <>
struct VertexLayout<VertexPos3D>
{
    static std::vector<VertexAttrib> GetAttributes();
};

// Specialization for VertexPosNormal3D
template <>
struct VertexLayout<VertexPosNormal3D>
{
    static std::vector<VertexAttrib> GetAttributes();
};

// Specialization for VertexPosNormalUV3D
template <>
struct VertexLayout<VertexPosNormalUV3D>
{
    static std::vector<VertexAttrib> GetAttributes();
};

// Specialization for VertexPosColor3D
template <>
struct VertexLayout<VertexPosColor3D>
{
    static std::vector<VertexAttrib> GetAttributes();
};

// Specialization for VertexPosColorUV3D
template <>
struct VertexLayout<VertexPosColorUV3D>
{
    static std::vector<VertexAttrib> GetAttributes();
};

// Specialization for VertexPosNormalTangentUV3D
template <>
struct VertexLayout<VertexPosNormalTangentUV3D>
{
    static std::vector<VertexAttrib> GetAttributes();
};

//==============================================================================
// 6. Explicit template instantiations (optional, to place template implementations
//    in a cpp file)
//==============================================================================

extern template class Vertex<VertexPos2D>;
extern template class Vertex<VertexPosUV2D>;
extern template class Vertex<VertexPosColorUV>;
extern template class Vertex<VertexPos3D>;
extern template class Vertex<VertexPosNormal3D>;
extern template class Vertex<VertexPosNormalUV3D>;
extern template class Vertex<VertexPosColor3D>;
extern template class Vertex<VertexPosColorUV3D>;
extern template class Vertex<VertexPosNormalTangentUV3D>;
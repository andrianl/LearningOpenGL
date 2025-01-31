#include "Vertex.h"

//==============================================================================
// 1. Implementation of GetAttributes methods for each VertexLayout specialization
//==============================================================================

// Specialization for VertexPos2D
std::vector<VertexAttrib> VertexLayout<VertexPos2D>::GetAttributes()
{
    return {// Position: location 0, 2 components (vec2), not normalized
        {0, 2, GL_FLOAT, sizeof(VertexPos2D), OFFSET_OF(VertexPos2D, pos), GL_FALSE}};
}

// Specialization for VertexPosUV2D
std::vector<VertexAttrib> VertexLayout<VertexPosUV2D>::GetAttributes()
{
    return {// Position: location 0, 2 components (vec2)
        {0, 2, GL_FLOAT, sizeof(VertexPosUV2D), OFFSET_OF(VertexPosUV2D, pos), GL_FALSE},
        // UV coordinates: location 1, 2 components (vec2)
        {1, 2, GL_FLOAT, sizeof(VertexPosUV2D), OFFSET_OF(VertexPosUV2D, uv), GL_FALSE}};
}

// Specialization for VertexPosColorUV
std::vector<VertexAttrib> VertexLayout<VertexPosColorUV>::GetAttributes()
{
    return {// Position: location 0, 2 components (vec2)
        {0, 2, GL_FLOAT, sizeof(VertexPosColorUV), OFFSET_OF(VertexPosColorUV, pos), GL_FALSE},
        // Color: location 1, 3 components (vec3)
        {1, 3, GL_FLOAT, sizeof(VertexPosColorUV), OFFSET_OF(VertexPosColorUV, color), GL_FALSE},
        // UV coordinates: location 2, 2 components (vec2)
        {2, 2, GL_FLOAT, sizeof(VertexPosColorUV), OFFSET_OF(VertexPosColorUV, uv), GL_FALSE}};
}

// Specialization for VertexPos3D
std::vector<VertexAttrib> VertexLayout<VertexPos3D>::GetAttributes()
{
    return {// Position: location 0, 3 components (vec3)
        {0, 3, GL_FLOAT, sizeof(VertexPos3D), OFFSET_OF(VertexPos3D, pos), GL_FALSE}};
}

// Specialization for VertexPosNormal3D
std::vector<VertexAttrib> VertexLayout<VertexPosNormal3D>::GetAttributes()
{
    return {// Position: location 0, 3 components (vec3)
        {0, 3, GL_FLOAT, sizeof(VertexPosNormal3D), OFFSET_OF(VertexPosNormal3D, pos), GL_FALSE},
        // Normal: location 1, 3 components (vec3)
        {1, 3, GL_FLOAT, sizeof(VertexPosNormal3D), OFFSET_OF(VertexPosNormal3D, normal), GL_FALSE}};
}

// Specialization for VertexPosNormalUV3D
std::vector<VertexAttrib> VertexLayout<VertexPosNormalUV3D>::GetAttributes()
{
    return {// Position: location 0, 3 components (vec3)
        {0, 3, GL_FLOAT, sizeof(VertexPosNormalUV3D), OFFSET_OF(VertexPosNormalUV3D, pos), GL_FALSE},
        // Normal: location 1, 3 components (vec3)
        {1, 3, GL_FLOAT, sizeof(VertexPosNormalUV3D), OFFSET_OF(VertexPosNormalUV3D, normal), GL_FALSE},
        // UV coordinates: location 2, 2 components (vec2)
        {2, 2, GL_FLOAT, sizeof(VertexPosNormalUV3D), OFFSET_OF(VertexPosNormalUV3D, uv), GL_FALSE}};
}

// Specialization for VertexPosColor3D
std::vector<VertexAttrib> VertexLayout<VertexPosColor3D>::GetAttributes()
{
    return {// Position: location 0, 3 components (vec3)
        {0, 3, GL_FLOAT, sizeof(VertexPosColor3D), OFFSET_OF(VertexPosColor3D, pos), GL_FALSE},
        // Color: location 1, 3 components (vec3)
        {1, 3, GL_FLOAT, sizeof(VertexPosColor3D), OFFSET_OF(VertexPosColor3D, color), GL_FALSE}};
}

// Specialization for VertexPosColorUV3D
std::vector<VertexAttrib> VertexLayout<VertexPosColorUV3D>::GetAttributes()
{
    return {// Position: location 0, 3 components (vec3)
        {0, 3, GL_FLOAT, sizeof(VertexPosColorUV3D), OFFSET_OF(VertexPosColorUV3D, pos), GL_FALSE},
        // Color: location 1, 3 components (vec3)
        {1, 3, GL_FLOAT, sizeof(VertexPosColorUV3D), OFFSET_OF(VertexPosColorUV3D, color), GL_FALSE},
        // UV coordinates: location 2, 2 components (vec2)
        {2, 2, GL_FLOAT, sizeof(VertexPosColorUV3D), OFFSET_OF(VertexPosColorUV3D, uv), GL_FALSE}};
}

// Specialization for VertexPosNormalTangentUV3D
std::vector<VertexAttrib> VertexLayout<VertexPosNormalTangentUV3D>::GetAttributes()
{
    return {// Position: location 0, 3 components (vec3)
        {0, 3, GL_FLOAT, sizeof(VertexPosNormalTangentUV3D), OFFSET_OF(VertexPosNormalTangentUV3D, pos), GL_FALSE},
        // Normal: location 1, 3 components (vec3)
        {1, 3, GL_FLOAT, sizeof(VertexPosNormalTangentUV3D), OFFSET_OF(VertexPosNormalTangentUV3D, normal), GL_FALSE},
        // Tangent: location 2, 3 components (vec3)
        {2, 3, GL_FLOAT, sizeof(VertexPosNormalTangentUV3D), OFFSET_OF(VertexPosNormalTangentUV3D, tangent), GL_FALSE},
        // UV coordinates: location 3, 2 components (vec2)
        {3, 2, GL_FLOAT, sizeof(VertexPosNormalTangentUV3D), OFFSET_OF(VertexPosNormalTangentUV3D, uv), GL_FALSE}};
}

//==============================================================================
// 2. Explicit instantiation of Vertex templates
//==============================================================================

template class Vertex<VertexPos2D>;
template class Vertex<VertexPosUV2D>;
template class Vertex<VertexPosColorUV>;
template class Vertex<VertexPos3D>;
template class Vertex<VertexPosNormal3D>;
template class Vertex<VertexPosNormalUV3D>;
template class Vertex<VertexPosColor3D>;
template class Vertex<VertexPosColorUV3D>;
template class Vertex<VertexPosNormalTangentUV3D>;

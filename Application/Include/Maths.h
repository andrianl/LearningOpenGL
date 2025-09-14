#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <string>
#include <unordered_map>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>
#include <random>
#include <cmath>

enum class CoordinateSystem
{
    XYZ,  // Forward: X, Right: Y, Up: Z
    XZY,  // Forward: X, Right: Z, Up: Y
    YXZ,  // Forward: Y, Right: X, Up: Z
    YZX,  // Forward: Y, Right: Z, Up: X
    ZXY,  // Forward: Z, Right: X, Up: Y
    ZYX   // Forward: Z, Right: Y, Up: X
};

float randomFloat01();

glm::vec3 randomNormalizedVec3();
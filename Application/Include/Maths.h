#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CoordinateSystem
{
    XYZ,  // Forward: X, Right: Y, Up: Z
    XZY,  // Forward: X, Right: Z, Up: Y
    YXZ,  // Forward: Y, Right: X, Up: Z
    YZX,  // Forward: Y, Right: Z, Up: X
    ZXY,  // Forward: Z, Right: X, Up: Y
    ZYX   // Forward: Z, Right: Y, Up: X
    // Додайте інші системи координат, якщо потрібно
};

glm::mat4 GetTransformationMatrix(CoordinateSystem from, CoordinateSystem to);
glm::vec3 ConvertCoordinateSystem(const glm::vec3& vec, CoordinateSystem from, CoordinateSystem to);
#include "Maths.h"

glm::mat4 GetTransformationMatrix(CoordinateSystem from, CoordinateSystem to)
{
    glm::mat4 transformation = glm::mat4(1.0f);  // Початкова матриця трансформації - одинична матриця

    // Перетворення з XYZ в іншу систему координат
    if (from == CoordinateSystem::XYZ && to == CoordinateSystem::XZY)
    {
        transformation = glm::rotate(transformation, glm::radians(-90.0f), glm::vec3(1, 0, 0));  // Поворот на -90 градусів навколо осі X
    }
    else if (from == CoordinateSystem::XYZ && to == CoordinateSystem::YXZ)
    {
        transformation = glm::rotate(transformation, glm::radians(90.0f), glm::vec3(0, 0, 1));  // Поворот на 90 градусів навколо осі Z
    }
    else if (from == CoordinateSystem::XYZ && to == CoordinateSystem::YZX)
    {
        transformation = glm::rotate(transformation, glm::radians(90.0f), glm::vec3(0, 1, 0));  // Поворот на 90 градусів навколо осі Y
    }
    else if (from == CoordinateSystem::XYZ && to == CoordinateSystem::ZXY)
    {
        transformation = glm::rotate(transformation, glm::radians(-90.0f), glm::vec3(0, 1, 0));  // Поворот на -90 градусів навколо осі Y
    }
    else if (from == CoordinateSystem::XYZ && to == CoordinateSystem::ZYX)
    {
        transformation = glm::rotate(transformation, glm::radians(180.0f), glm::vec3(0, 1, 0));  // Поворот на 180 градусів навколо осі Y
    }
    // Додаємо інші необхідні перетворення для різних систем координат
    // ...

    return transformation;
}

glm::vec3 ConvertCoordinateSystem(const glm::vec3& vec, CoordinateSystem from, CoordinateSystem to)
{
    glm::mat4 transformation = GetTransformationMatrix(from, to);
    glm::vec4 transformedVec = transformation * glm::vec4(vec, 1.0f);
    return glm::vec3(transformedVec);
}

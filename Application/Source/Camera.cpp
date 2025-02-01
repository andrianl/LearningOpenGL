#include "Camera.h"
#include <glm/gtc/constants.hpp>  // для константи PI

// Повний конструктор з усіма параметрами
Camera::Camera(glm::vec3 position, glm::vec3 target, float aspectRatio, float nearPlane, float farPlane, float focalLength, float aperture)
    : position(position), target(target), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), focalLength(focalLength),
      aperture(aperture)
{
    UpdateFOV();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

// Конструктор без діафрагми (апертура)
Camera::Camera(glm::vec3 position, glm::vec3 target, float aspectRatio, float nearPlane, float farPlane, float focalLength)
    : position(position), target(target), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), focalLength(focalLength),
      aperture(2.8f)  // Значення апертури за замовчуванням
{
    UpdateFOV();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

// Конструктор без фокусної відстані і діафрагми
Camera::Camera(glm::vec3 position, glm::vec3 target, float aspectRatio, float nearPlane, float farPlane)
    : position(position), target(target), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), focalLength(50.0f),
      aperture(2.8f)  // Значення фокусної відстані та апертури за замовчуванням
{
    UpdateFOV();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

// Конструктор тільки з позицією, ціллю і аспектним співвідношенням
Camera::Camera(glm::vec3 position, glm::vec3 target, float aspectRatio)
    : position(position), target(target), aspectRatio(aspectRatio), nearPlane(0.1f), farPlane(100.0f), focalLength(50.0f),
      aperture(2.8f)  // Значення за замовчуванням для інших параметрів
{
    UpdateFOV();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

// Конструктор за замовчуванням
Camera::Camera()
    : position(glm::vec3(0.0f, 0.0f, 5.0f)), target(glm::vec3(0.0f, 0.0f, 0.0f)), aspectRatio(16.0f / 9.0f), nearPlane(0.1f),
      farPlane(100.0f), focalLength(50.0f), aperture(2.8f)
{
    UpdateFOV();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

// Інші методи залишаються без змін

void Camera::SetPosition(const glm::vec3& position)
{
    this->position = position;
    UpdateViewMatrix();
}

void Camera::SetTarget(const glm::vec3& target)
{
    this->target = target;
    UpdateViewMatrix();
}

void Camera::SetAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void Camera::SetNearPlane(float nearPlane)
{
    this->nearPlane = nearPlane;
    UpdateProjectionMatrix();
}

void Camera::SetFarPlane(float farPlane)
{
    this->farPlane = farPlane;
    UpdateProjectionMatrix();
}

void Camera::SetFocalLength(float focalLength)
{
    this->focalLength = focalLength;
    UpdateFOV();
    UpdateProjectionMatrix();
}

void Camera::SetAperture(float aperture)
{
    this->aperture = aperture;
    UpdateFOV();               // Оновлення поля зору (FOV) при зміні діафрагми
    UpdateProjectionMatrix();  // Оновлення проекційної матриці
}

void Camera::LookAt(const glm::vec3& target)
{
    this->target = target;
    UpdateViewMatrix();
}

void Camera::SetFOV(float fov)
{
    this->fov = fov;
    UpdateFocalLength();
    UpdateProjectionMatrix();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return projectionMatrix;
}

void Camera::UpdateViewMatrix()
{
    viewMatrix = glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));  // Позиція, ціль, вектор вгору
}

void Camera::UpdateProjectionMatrix()
{
    projectionMatrix = glm::perspective(
        glm::radians(fov), aspectRatio, nearPlane, farPlane);  // Поле зору, аспектне співвідношення, ближня та дальня площини обрізання
}

void Camera::UpdateFOV()
{
    // Припускаємо, що висота сенсора 24 мм для спрощення
    float sensorHeight = 24.0f;
    fov = 2.0f * glm::degrees(glm::atan(sensorHeight / (2.0f * focalLength)));  // В градусах
}

void Camera::UpdateFocalLength()
{
    // Припускаємо, що висота сенсора 24 мм для спрощення
    float sensorHeight = 24.0f;
    focalLength = sensorHeight / (2.0f * glm::tan(glm::radians(fov) / 2.0f));  // Обчислення фокусної відстані на основі поля зору
}

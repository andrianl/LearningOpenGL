#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
    // Повний конструктор з усіма параметрами
    Camera(glm::vec3 position, glm::vec3 target, float aspectRatio, float nearPlane, float farPlane, float focalLength, float aperture);

    // Конструктор без діафрагми (апертура)
    Camera(glm::vec3 position, glm::vec3 target, float aspectRatio, float nearPlane, float farPlane, float focalLength);

    // Конструктор без фокусної відстані і діафрагми
    Camera(glm::vec3 position, glm::vec3 target, float aspectRatio, float nearPlane, float farPlane);

    // Конструктор тільки з позицією, ціллю і аспектним співвідношенням
    Camera(glm::vec3 position, glm::vec3 target, float aspectRatio);

    // Конструктор за замовчуванням
    Camera();

    // Отримання матриці перегляду (View matrix)
    glm::mat4 GetViewMatrix() const;

    // Отримання проекційної матриці (Projection matrix)
    glm::mat4 GetProjectionMatrix() const;

    // Методи для управління камерою
    void SetPosition(const glm::vec3& position);
    void SetTarget(const glm::vec3& target);
    void SetAspectRatio(float aspectRatio);
    void SetNearPlane(float nearPlane);
    void SetFarPlane(float farPlane);
    void SetFocalLength(float focalLength);
    void SetAperture(float aperture);
    void LookAt(const glm::vec3& target);
    void SetFOV(float fov);

    // Позиція камери
    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetTarget() const { return target; }
    float GetFocalLength() const { return focalLength; }
    float GetAperture() const { return aperture; }
    float GetFOV() const { return fov; }

private:
    glm::vec3 position;  // Позиція камери
    glm::vec3 target;    // Ціль камери
    float aspectRatio;   // Аспектне співвідношення
    float nearPlane;     // Ближня площина обрізання
    float farPlane;      // Дальня площина обрізання
    float focalLength;   // Фокусна відстань
    float aperture;      // Діафрагма
    float fov;           // Поле зору (Field of View)

    glm::mat4 viewMatrix;        // Матриця перегляду
    glm::mat4 projectionMatrix;  // Проекційна матриця

    // Оновлення матриць при зміні параметрів
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
    void UpdateFOV();  // Оновлення поля зору на основі фокусної відстані та діафрагми
    void UpdateFocalLength();  // Оновлення фокусної відстані на основі поля зору
};

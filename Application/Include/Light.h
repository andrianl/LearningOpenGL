#pragma once
#include "glm/glm.hpp"
#include <Shaders.h>

class Light 
{

public:
	Light() = delete;

	glm::vec3 GetColor() const { return Color; }
	float GetIntensity() const { return Intensity; }

	inline void SetColor(const glm::vec3& newColor) { Color = newColor; }
	inline void SetIntensity(float newIntensity) { Intensity = newIntensity; }

private:
	glm::vec3 Ñolor{ 1.0f }; 
	float Intensity = 1.0f;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight() = default;

	glm::vec3 GetDirection() const { return Direction; }
	inline void SetDirection(const glm::vec3& newDirection) { Direction = glm::normalize(newDirection); }

	void Apply(Shader& shader) const 
	{
		shader.SetVec3("DirectionalLight.Direction", Direction);
		shader.SetVec3("DirectionalLight.Color", GetColor() * GetIntensity());
	}

private:
	glm::vec3 Direction{ -0.2f, -1.0f, -0.3f };
};

class PointLight : public Light
{
public:
	PointLight() = default;

	glm::vec3 GetPosition() const { return position; }
	float GetConstant() const { return constant; }
	float GetLinear() const { return linear; }
	float GetQuadratic() const { return quadratic; }

	inline void SetPosition(const glm::vec3& newPosition) { position = newPosition; }
	inline void SetAttenuation(float newConstant, float newLinear, float newQuadratic) {
		constant = newConstant;
		linear = newLinear;
		quadratic = newQuadratic;
	}

	void Apply(Shader& shader) const {
		shader.SetVec3("pointLight.position", position);
		shader.SetVec3("pointLight.color", GetColor() * GetIntensity());
		shader.SetFloat("pointLight.constant", constant);
		shader.SetFloat("pointLight.linear", linear);
		shader.SetFloat("pointLight.quadratic", quadratic);
	}

private:
	glm::vec3 position{ 0.0f };
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

class SpotLight : public PointLight
{
public:
	SpotLight() = default;

	glm::vec3 GetDirection() const { return direction; }
	float GetCutOff() const { return cutOff; }
	float GetOuterCutOff() const { return outerCutOff; }

	inline void SetDirection(const glm::vec3& newDirection) { direction = glm::normalize(newDirection); }
	inline void SetCutOff(float newCutOff) { cutOff = glm::cos(glm::radians(newCutOff)); }
	inline void SetOuterCutOff(float newOuterCutOff) { outerCutOff = glm::cos(glm::radians(newOuterCutOff)); }

	void Apply(Shader& shader) const {
		shader.SetVec3("spotLight.position", GetPosition());
		shader.SetVec3("spotLight.direction", direction);
		shader.SetVec3("spotLight.color", GetColor() * GetIntensity());
		shader.SetFloat("spotLight.constant", GetConstant());
		shader.SetFloat("spotLight.linear", GetLinear());
		shader.SetFloat("spotLight.quadratic", GetQuadratic());
		shader.SetFloat("spotLight.cutOff", cutOff);
		shader.SetFloat("spotLight.outerCutOff", outerCutOff);
	}

private:
	glm::vec3 direction{ 0.0f, -1.0f, 0.0f };
	float cutOff = glm::cos(glm::radians(12.5f));
	float outerCutOff = glm::cos(glm::radians(17.5f));
};

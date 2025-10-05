#pragma once
#include "glm/glm.hpp"
#include <Shaders.h>
#include <Platform.h>

class Light
{
public:
	//Light() = delete;

	glm::vec3 GetColor() const { return Color; }
	float GetIntensity() const { return Intensity; }

	inline void SetColor(const glm::vec3& NewColor) { Color = NewColor; }
	inline void SetIntensity(float NewIntensity) { Intensity = NewIntensity; }

	/*virtual void Apply(Shader& ShaderProgram) const = 0; */ // Make Light an abstract class if you want to enforce implementation in derived classes

protected:
	glm::vec3 Color{ 1.0f };
	float Intensity = 1.0f;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight() = default;

	glm::vec3 GetDirection() const { return Direction; }
	inline void SetDirection(const glm::vec3& NewDirection) { Direction = glm::normalize(NewDirection); }

	void Apply(GraphicsShader* ShaderProgram, int LightIndex) const
	{
		const std::string indexStr = "[" + std::to_string(LightIndex) + "]";
		ShaderProgram->SetVec3("DirectionalLights" + indexStr + ".Color", GetColor());
		ShaderProgram->SetFloat("DirectionalLights" + indexStr + ".Intensity", GetIntensity());
		ShaderProgram->SetVec3("DirectionalLights" + indexStr + ".Direction", Direction);
	}

private:
	glm::vec3 Direction{ -0.2f, -1.0f, -0.3f };
};

class PointLight : public Light
{
public:
	PointLight() = default;

	glm::vec3 GetPosition() const { return Position; }
	float GetConstant() const { return Constant; }
	float GetLinear() const { return Linear; }
	float GetQuadratic() const { return Quadratic; }

	inline void SetPosition(const glm::vec3& NewPosition) { Position = NewPosition; }
	inline void SetAttenuation(float NewConstant, float NewLinear, float NewQuadratic) {
		Constant = NewConstant;
		Linear = NewLinear;
		Quadratic = NewQuadratic;
	}

	void Apply(GraphicsShader* ShaderProgram, int LightIndex) const
	{
		const std::string indexStr = "[" + std::to_string(LightIndex) + "]";
		ShaderProgram->SetVec3("PointLights" + indexStr + ".Color", GetColor());
		ShaderProgram->SetFloat("PointLights" + indexStr + ".Intensity", GetIntensity());
		ShaderProgram->SetVec3("PointLights" + indexStr + ".Position", Position);
		ShaderProgram->SetFloat("PointLights" + indexStr + ".Constant", Constant);
		ShaderProgram->SetFloat("PointLights" + indexStr + ".Linear", Linear);
		ShaderProgram->SetFloat("PointLights" + indexStr + ".Quadratic", Quadratic);
	}

private:
	glm::vec3 Position{ 0.0f };
	float Constant = 1.0f;
	float Linear = 0.09f;
	float Quadratic = 0.032f;
};

class SpotLight : public PointLight
{
public:
	SpotLight() = default;

	glm::vec3 GetDirection() const { return Direction; }
	float GetCutOff() const { return CutOff; }
	float GetOuterCutOff() const { return OuterCutOff; }

	inline void SetDirection(const glm::vec3& NewDirection) { Direction = glm::normalize(NewDirection); }
	inline void SetCutOff(float NewCutOff) { CutOff = glm::cos(glm::radians(NewCutOff)); }
	inline void SetOuterCutOff(float NewOuterCutOff) { OuterCutOff = glm::cos(glm::radians(NewOuterCutOff)); }

	void Apply(GraphicsShader* ShaderProgram, int LightIndex) const
	{
		const std::string indexStr = "[" + std::to_string(LightIndex) + "]";
		ShaderProgram->SetVec3("SpotLights" + indexStr + ".Color", GetColor());
		ShaderProgram->SetFloat("SpotLights" + indexStr + ".Intensity", GetIntensity());
		ShaderProgram->SetVec3("SpotLights" + indexStr + ".Position", GetPosition());
		ShaderProgram->SetFloat("SpotLights" + indexStr + ".Constant", GetConstant());
		ShaderProgram->SetFloat("SpotLights" + indexStr + ".Linear", GetLinear());
		ShaderProgram->SetFloat("SpotLights" + indexStr + ".Quadratic", GetQuadratic());
		ShaderProgram->SetVec3("SpotLights" + indexStr + ".Direction", Direction);
		ShaderProgram->SetFloat("SpotLights" + indexStr + ".CutOff", CutOff);
		ShaderProgram->SetFloat("SpotLights" + indexStr + ".OuterCutOff", OuterCutOff);
	}

private:
	glm::vec3 Direction{ 0.0f, -1.0f, 0.0f };
	float CutOff = glm::cos(glm::radians(12.5f));
	float OuterCutOff = glm::cos(glm::radians(17.5f));
};
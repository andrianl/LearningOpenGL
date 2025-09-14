#include "Maths.h"

static std::mt19937 rng(std::random_device{}());
static std::uniform_real_distribution<float> dist01(0.0f, 1.0f);

float randomFloat01()
{
	return dist01(rng);
}

glm::vec3 randomNormalizedVec3()
{
	float theta = dist01(rng) * glm::two_pi<float>(); 
	float phi = glm::acos(2.0f * dist01(rng) - 1.0f);   

	float x = glm::sin(phi) * glm::cos(theta);
	float y = glm::sin(phi) * glm::sin(theta);
	float z = glm::cos(phi);

	return {x, y, z}; 
}


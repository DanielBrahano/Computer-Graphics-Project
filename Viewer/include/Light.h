#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Light
{
private:
	glm::vec3 AmbientColor;
	glm::vec3 DiffuseColor;
	glm::vec3 SpecularColor;

	glm::vec3 Translation;

public:
	Light();
};




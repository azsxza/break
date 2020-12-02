#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "FGameObject.h"
#include "FTexture.h"

const glm::vec2 SIZE(60, 20);
const glm::vec2 VELOCITY(0, 150);

class FPowerUp : public FGameObject
{
public:
	string Type;
	GLfloat Duration;
	GLboolean Activated;

	FPowerUp(string Type, glm::vec3 Color, GLfloat duration, glm::vec2 Position, FTexture Texture);
};
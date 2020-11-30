#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "FTexture.h"
#include "FSprite.h"

class FGameObject
{
public:
	glm::vec2 Position, Size, Velocity;
	glm::vec3 Color;
	GLfloat Rotation;
	GLboolean Destroyed;
	GLboolean IsSolid;

	FTexture Texture;

	FGameObject();
	FGameObject(glm::vec2 Position, glm::vec2 Size, FTexture Texture, glm::vec3 Color = glm::vec3(1.0f), glm::vec2 Velocity = glm::vec2(0.0f));

	virtual void Draw(FSprite& Sprite);
};
#pragma once
#include <glm/glm.hpp>
#include "FGameObject.h"

class FBallObject :public FGameObject
{
public:
	GLfloat Radius;
	GLboolean Stuck;

	GLboolean Sticky;
	GLboolean PassThrough;

	FBallObject();
	FBallObject(glm::vec2 Pos, float Radius, glm::vec2 Velocity, FTexture Sprite);

	glm::vec2 Move(float DeltaTime, GLuint WindowWidth);
	void Reset(glm::vec2 Position, glm::vec2 Velocity);

};

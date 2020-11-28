#pragma once
#include "FShader.h"
#include "FTexture.h"
#include <glm/glm.hpp>

class FSprite
{
public:
	FSprite(FShader& Shader);
	~FSprite();

	void Draw(FTexture& Texture, glm::vec2 Position, glm::vec2 Size = glm::vec2(10, 10),
		GLfloat Rotate = 0.0f, glm::vec3 Color = glm::vec3(1.0f));
private:
	FShader Shader;
	GLuint QuadVAO;

	void InitRenderData();
};
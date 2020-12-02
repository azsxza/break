#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "FTexture.h"
#include "FSprite.h"
#include "FShader.h"

class FPostProcesser
{
public:
	FShader PostProcessShader;
	FTexture Texture;
	GLuint Width, Height;
	GLboolean Confuse, Chaos, Shake;

	FPostProcesser(FShader Shader, GLuint Width, GLuint Height);
	void BeginRender();
	void EndRender();
	void Render(GLfloat Time);

private:
	GLuint MSFBO, FBO;
	GLuint RBO;
	GLuint VAO;
	void InitRenderData();
};
#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "FShader.h"
#include "FTexture.h"
#include "FGameObject.h"

using namespace std;

struct FPaticle
{
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	FPaticle();
};

class FPaticleGenerator
{
public:
	FPaticleGenerator(FShader Shader, FTexture Texture, GLuint Amount);
	void Update(GLfloat DeltaTime, FGameObject& Object, GLuint NewParticles, glm::vec2 Offset = glm::vec2(0.0));
	void Draw();

private:
	vector<FPaticle> Particles;
	GLuint Amount;
	FShader Shader;
	FTexture Texture;
	GLuint VAO;
	void Init();
	GLuint FirstUnusedParticle();

	void RespwnParticle(FPaticle& Particle, FGameObject& Object, glm::vec2 Offset = glm::vec2(0.0f));
};
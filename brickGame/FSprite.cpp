#include "FSprite.h"

FSprite::FSprite(FShader & Shader)
{
}

FSprite::~FSprite()
{
}

void FSprite::Draw(FTexture & Texture, glm::vec2 Position, glm::vec2 Size, GLfloat Rotate, glm::vec3 Color)
{
	this->Shader.Use();
}

void FSprite::InitRenderData()
{
	GLuint VBO;
	GLfloat vertices[] = {
		// Î»ÖÃ     // ÎÆÀí
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->QuadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->QuadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

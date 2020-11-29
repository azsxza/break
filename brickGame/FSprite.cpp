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
	glm::mat4 Model;
	Model = glm::translate(Model, glm::vec3(Position, 0.0f));
	Model = glm::translate(Model, glm::vec3(0.5f*Size, 0.0f));
	Model = glm::rotate(Model, Rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	Model = glm::translate(Model, glm::vec3(-0.5f*Size, 0.0f));
	Model = glm::scale(Model, glm::vec3(Size, 0.0f));

	this->Shader.SetMat4("Model", Model);
	this->Shader.SetVec3("SpriteColor", Color);
	Texture.Active(GL_TEXTURE0);
	glBindVertexArray(this->QuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
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

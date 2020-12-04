#pragma once
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "FTexture.h"
#include "FShader.h"

struct FCharacter
{
	GLuint Texture;
	glm::vec2 Size;
	glm::vec2 Bearing;
	GLuint Advance;
};

class FTextRender
{
public:
	unordered_map<GLchar, FCharacter> Characters;
	FShader TextShader;

	FTextRender(GLuint Width, GLuint Height);
	void Load(const string& Font, GLuint FontSize);
	void RenderText(string Text, GLfloat X, GLfloat Y, GLfloat Scale, glm::vec3 Color = glm::vec3(1.0f));
private:
	GLuint VAO, VBO;
};
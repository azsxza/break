#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include "stbi_image.h"
using namespace std;

class FTexture
{
public:
	unsigned int ID;
	int Width;
	int Height;
	int Channels;
	GLuint Format;
	GLuint Type;
	GLuint WrapS;
	GLuint WrapT;
	GLuint FilterMin;
	GLuint FilterMax;
	
	FTexture(int Width, int Height, GLuint Format = GL_RGB, GLuint Type = GL_UNSIGNED_BYTE, 
		GLuint FilterMin = GL_LINEAR, GLuint FileterMax = GL_LINEAR);
	FTexture(string ImagePath, GLuint Format = 0, GLuint Type = GL_UNSIGNED_BYTE, bool Filp = false,
		GLuint WrapS = GL_REPEAT, GLuint WrapT = GL_REPEAT, 
		GLuint FilterMin = GL_LINEAR, GLuint FileterMax = GL_LINEAR);
	FTexture();
	void Generate(GLuint Width, GLuint Height, GLubyte* data);
	void Active(GLuint Index);
};
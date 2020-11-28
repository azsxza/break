#include "FTexture.h"

FTexture::FTexture(int Width, int Height, GLuint Format, GLuint Type,
	GLuint FilterMin, GLuint FilterMax)
{
	this->WrapS = WrapS;
	this->WrapT = WrapT;
	this->FilterMin = FilterMin;
	this->FilterMax = FilterMax;
	this->Format = Format;
	this->Type = Type;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Format, this->Width, this->Height, 0, this->Format, this->Type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
	glBindTexture(GL_TEXTURE_2D, 0);
}

FTexture::FTexture(string ImagePath, GLuint Format, GLuint Type, bool Filp,
	GLuint WrapS, GLuint WrapT,
	GLuint FilterMin, GLuint FilterMax)
{
	this->WrapS = WrapS;
	this->WrapT = WrapT;
	this->FilterMin = FilterMin;
	this->FilterMax = FilterMax;
	this->Format = Format;
	this->Type = Type;
	stbi_set_flip_vertically_on_load(Filp);

	unsigned char* data = stbi_load(ImagePath.c_str(), &Width, &Height, &Channels, 0);
	if (!data)
	{
		cout << "can not load image:" << ImagePath << endl;
		return;
	}

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
	if (!Format)
	{
		if (Channels == 3)
			this->Format = GL_RGB;
		if (Channels == 4)
			this->Format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, this->Format, Width, Height, 0, this->Format, this->Type, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data); 
	glBindTexture(GL_TEXTURE_2D, 0);

}

FTexture::FTexture()
{
	this->Width = 0;
	this->Height = 0;
	this->Format = GL_RGB;
	this->Type = GL_UNSIGNED_BYTE;
	this->WrapS = GL_REPEAT;
	this->WrapT = GL_REPEAT;
	this->FilterMax = GL_LINEAR;
	this->FilterMin = GL_LINEAR;

	glGenTextures(1, &ID);
}

void FTexture::Generate(GLuint Width, GLuint Height, GLubyte * data)
{
	this->Width = Width;
	this->Height = Height;
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Format, this->Width, this->Height, 0, this->Format, this->Type, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FTexture::Active(GLuint Index)
{
	glActiveTexture(Index);
	glBindTexture(GL_TEXTURE_2D, ID);
}

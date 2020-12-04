#include "FTextRender.h"
#include "FResourceManager.h"
#include <ft2build.h>
#include FT_FREETYPE_H

FTextRender::FTextRender(GLuint Width, GLuint Height)
{
	this->TextShader = FResourceManager::LoadShader("text.vert", "text.frag", "", "text");
	this->TextShader.Use();
	this->TextShader.SetMat4("projection", glm::ortho(0.0f, (float)Width, (float)Height, 0.0f));
	this->TextShader.SetInt("text", 0);

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, NULL, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void FTextRender::Load(const string & Font, GLuint FontSize)
{
	this->Characters.clear();
	FT_Library Ft;
	if (FT_Init_FreeType(&Ft))
		cout << "ERROR::FREETYPE:Failed to load lib" << endl;
	FT_Face Face;
	if (FT_New_Face(Ft, Font.c_str(), 0, &Face))
		cout << "ERROR::FREETYPE:Failed to load font" << endl;
	FT_Set_Pixel_Sizes(Face, 0, FontSize);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(Face, c, FT_LOAD_RENDER))
		{
			cout << "ERROR::FREETYPE:Failed to load Glyph" << endl;
			continue;
		}
		GLuint Texture;
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Face->glyph->bitmap.width, Face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, Face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		FCharacter Character = { Texture,glm::ivec2(Face->glyph->bitmap.width,Face->glyph->bitmap.rows),glm::ivec2(Face->glyph->bitmap_left,Face->glyph->bitmap_top),Face->glyph->advance.x };
		Characters.insert(pair<GLchar, FCharacter>(c, Character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(Face);
	FT_Done_FreeType(Ft);
}

void FTextRender::RenderText(string Text, GLfloat X, GLfloat Y, GLfloat Scale, glm::vec3 Color)
{
	this->TextShader.Use();
	this->TextShader.SetVec3("textColor", Color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = Text.begin(); c != Text.end(); c++)
	{
		FCharacter ch = Characters[*c];

		GLfloat xpos = X + ch.Bearing.x * Scale;
		GLfloat ypos = Y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * Scale;

		GLfloat w = ch.Size.x * Scale;
		GLfloat h = ch.Size.y * Scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.Texture);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph
		X += (ch.Advance >> 6) * Scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

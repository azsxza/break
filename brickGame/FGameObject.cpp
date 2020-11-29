#include "FGameObject.h"

FGameObject::FGameObject()
{
	this->Position = glm::vec2(0.0f);
	this->Size = glm::vec2(1.0f);
	this->Color = glm::vec3(1.0f);
	this->Velocity = glm::vec2(0.0f);
	this->Rotation = 0.0f;
	this->Texture = FTexture();
	this->Destroyed = false;
}

FGameObject::FGameObject(glm::vec2 Position, glm::vec2 Size, FTexture Texture, glm::vec3 Color, glm::vec2 Velocity)
{
	this->Position = Position;
	this->Size = Size;
	this->Color = Color;
	this->Velocity = Velocity;
	this->Rotation = 0.0f;
	this->Texture = Texture;
	this->Destroyed = false;
}

void FGameObject::Draw(FSprite & Sprite)
{
	Sprite.Draw(this->Texture, this->Position, this->Size, this->Rotation, this->Color);
}

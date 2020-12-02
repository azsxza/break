#include "FPowerUp.h"

FPowerUp::FPowerUp(string Type, glm::vec3 Color, GLfloat Duration, glm::vec2 Position, FTexture Texture):FGameObject(Position,SIZE
,Texture,Color,VELOCITY)
{
	this->Type = Type;
	this->Duration = Duration;
}

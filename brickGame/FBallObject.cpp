#include "FBallObject.h"

FBallObject::FBallObject()
{
	FGameObject();
	this->Radius = 12.5f;
	this->Stuck = true;
	this->Sticky = false;
	this->PassThrough = false;
}

FBallObject::FBallObject(glm::vec2 Pos, float Radius, glm::vec2 Velocity, FTexture Texture):FGameObject(Pos, glm::vec2(Radius * 2), Texture, glm::vec3(1.0f), Velocity)
{
	
	this->Radius = Radius;
	this->Stuck = true;
	this->Sticky = false;
	this->PassThrough = false;
}

glm::vec2 FBallObject::Move(float DeltaTime, GLuint WindowWidth)
{
	if (!this->Stuck)
	{
		this->Position += this->Velocity*DeltaTime;
		if (this->Position.x <= 0.0f)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = 0.0f;
		}
		else if (this->Position.x + this->Size.x > WindowWidth)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = WindowWidth - this->Size.x;
		}
		if (this->Position.y <= 0.0f)
		{
			this->Velocity.y = -this->Velocity.y;
			this->Position.y = 0.0f;
		}
	}
	return this->Position;
}

void FBallObject::Reset(glm::vec2 Position, glm::vec2 Velocity)
{
	this->Position = Position;
	this->Velocity = Velocity;
	this->Stuck = true;
}

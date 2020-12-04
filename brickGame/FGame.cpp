#include "FGame.h"

#include <irrKlang.h>
#include <algorithm>
using namespace irrklang;

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;
float ShakeTime = 0.0f;
FGameObject* Player;
FSprite* Renderer;
FBallObject* Ball;
FPaticleGenerator* Particle;
FPostProcesser* PostProcessor;
FTextRender* Text;

ISoundEngine* SoundEngine;

FGame::FGame(GLuint Width, GLuint Height)
{
	State = GAME_MENU;
	this->Width = Width;
	this->Height = Height;
	this->Lives = 3;
}

FGame::~FGame()
{

}


void FGame::Init()
{
	FResourceManager::LoadShader("sprite.vert", "sprite.frag", "", "sprite");
	FResourceManager::LoadShader("particle.vert", "particle.frag", "", "particle");
	FResourceManager::LoadShader("postprocess.vert", "postprocess.frag", "", "postprocess");
	glm::mat4 Projection = glm::ortho(0.0f, (GLfloat)this->Width, (GLfloat)this->Height, 0.0f, -1.0f, 1.0f);
	FResourceManager::GetShader("sprite").Use().SetInt("Image", 0);
	FResourceManager::GetShader("sprite").SetMat4("Projection", Projection);
	FResourceManager::GetShader("particle").Use().SetInt("Image", 0);
	FResourceManager::GetShader("particle").SetMat4("Projection", Projection);

	FResourceManager::LoadTexture("awesomeface.png", "face");
	FResourceManager::LoadTexture("background.jpg", "background");
	FResourceManager::LoadTexture("block.png", "block");
	FResourceManager::LoadTexture("block_solid.png", "blockSolid");
	FResourceManager::LoadTexture("paddle.png", "paddle");
	FResourceManager::LoadTexture("particle.png", "particle");
	FResourceManager::LoadTexture("powerup_chaos.png", "power_chaos");
	FResourceManager::LoadTexture("powerup_confuse.png", "power_confuse");
	FResourceManager::LoadTexture("powerup_increase.png", "power_size");
	FResourceManager::LoadTexture("powerup_passthrough.png", "power_pass");
	FResourceManager::LoadTexture("powerup_speed.png", "power_speed");
	FResourceManager::LoadTexture("powerup_sticky.png", "power_sticky");

	Renderer = new FSprite(FResourceManager::GetShader("sprite"));
	Particle = new FPaticleGenerator(FResourceManager::GetShader("particle"), FResourceManager::GetTexture("particle"), 500);
	PostProcessor = new FPostProcesser(FResourceManager::GetShader("postprocess"), this->Width, this->Height);

	FGameLevel LevelOne;
	LevelOne.Load("LevelOne.txt", this->Width, this->Height/2);
	FGameLevel LevelTwo;
	LevelTwo.Load("LevelTwo.txt", this->Width, this->Height/2);
	FGameLevel LevelThree;
	LevelThree.Load("LevelThree.txt", this->Width, this->Height/2);
	FGameLevel LevelFour;
	LevelFour.Load("LevelFour.txt", this->Width, this->Height/2);
	this->Levels.push_back(LevelOne);
	this->Levels.push_back(LevelTwo);
	this->Levels.push_back(LevelThree);
	this->Levels.push_back(LevelFour);
	this->Level = 0;

	glm::vec2 PlayerPos(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = new FGameObject(PlayerPos, PLAYER_SIZE, FResourceManager::GetTexture("paddle"));
	glm::vec2 BallPos = PlayerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new FBallObject(BallPos,BALL_RADIUS,INITIAL_BALL_VELOCITY,FResourceManager::GetTexture("face"));
	SoundEngine = createIrrKlangDevice();
	SoundEngine->play2D("breakout.mp3");

	Text = new FTextRender(this->Width, this->Height);
	Text->Load("OCRAEXT.TTF", 24);

}

void FGame::Update(GLfloat DeltaTime)
{
	Ball->Move(DeltaTime, this->Width);
	this->DoCollision();
	Particle->Update(DeltaTime, *Ball, 2, glm::vec2(Ball->Radius / 2));
	if (Ball->Position.y >= this->Height)
	{
		this->Lives--;
		if (this->Lives == 0)
		{
			this->ResetLevel();
			this->State = GAME_MENU;
		}
		this->ResetPlayer();
	}
	if (ShakeTime > 0)
	{
		ShakeTime -= DeltaTime;
		if (ShakeTime < 0)
			PostProcessor->Shake = false;
	}
	this->UpdatePowerUps(DeltaTime);
}

void FGame::ProcessInput(GLfloat DeltaTime)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat Velocity = PLAYER_VELOCITY * DeltaTime;
		if (this->bKeys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= Velocity;
				if (Ball->Stuck)
				{
					Ball->Position.x -= Velocity;
				}
			}
		}
		if (this->bKeys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += Velocity;
				if (Ball->Stuck)
				{
					Ball->Position.x += Velocity;
				}
			}
		}
		if (this->bKeys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}
	else if (this->State == GAME_MENU)
	{
		if (this->bKeys[GLFW_KEY_ENTER] && !this->bKeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->bKeysProcessed[GLFW_KEY_ENTER] = true;
		}
		if (this->bKeys[GLFW_KEY_W] && !this->bKeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->bKeysProcessed[GLFW_KEY_W] = true;
		}
		if (this->bKeys[GLFW_KEY_S] && !this->bKeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
				this->Level--;
			else
				this->Level = 3;
			this->bKeysProcessed[GLFW_KEY_S] = true;
		}
	}
}

void FGame::Render()
{
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU)
	{
		PostProcessor->BeginRender();
		Renderer->Draw(FResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		this->Levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);
		Particle->Draw();
		for (auto& PowerUp : this->PowerUps)
		{
			if (!PowerUp.Destroyed)
				PowerUp.Draw(*Renderer);
		}
		Ball->Draw(*Renderer);


		PostProcessor->EndRender();
		PostProcessor->Render(glfwGetTime());
		//cout << (int)PostProcessor->Confuse << endl;
		//cout << (int)Ball->PassThrough << endl;

		Text->RenderText("Lives:" + to_string(this->Lives), 5.0f, 5.0f, 1.0f);
	}
	if (this->State == GAME_MENU)
	{
		Text->RenderText("Press Enter to Start", 250.0f, Height / 2, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 1.0f);
	}
}

void FGame::DoCollision()
{
	for (auto& Box : this->Levels[this->Level].Bricks)
	{
		if (!Box.Destroyed)
		{
			FCollision Collision = CheckCollision(*Ball, Box);
			if (get<0>(Collision))
			{
				if (!Box.IsSolid)
				{
					Box.Destroyed = true;
					this->SpawnPowerUps(Box);
					SoundEngine->play2D("bleep.mp3");
				}
				else
				{
					ShakeTime = 0.05f;
					PostProcessor->Shake = true;
					SoundEngine->play2D("solid.wav");
				}
				Direction Dir = get<1>(Collision);
				glm::vec2 DiffVector = get<2>(Collision);
				if (!(Ball->PassThrough && !Box.IsSolid))
				{
					if (Dir == LEFT || Dir == RIGHT)
					{
						Ball->Velocity.x = -Ball->Velocity.x;
						GLfloat Penetration = Ball->Radius - abs(DiffVector.x);
						if (Dir == LEFT)
						{
							Ball->Position.x += Penetration;
						}
						else
						{
							Ball->Position.x -= Penetration;
						}
					}
					else
					{
						Ball->Velocity.y = -Ball->Velocity.y;
						GLfloat Penetration = Ball->Radius - abs(DiffVector.x);
						if (Dir == UP)
						{
							Ball->Position.y -= Penetration;
						}
						else
						{
							Ball->Position.y += Penetration;
						}
					}
				}
			}
		}
	}
	FCollision Collision = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck&&get<0>(Collision))
	{
		GLfloat CenterBoard = Player->Position.x + Player->Size.x / 2;
		GLfloat Distance = Ball->Position.x + Ball->Radius - CenterBoard;
		GLfloat Percentage = Distance / (Player->Size.x / 2);
		GLfloat Strength = 2.0f;
		GLfloat OLength = glm::length(Ball->Velocity);
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x*Percentage*Strength;
		Ball->Velocity.y = -abs(Ball->Velocity.y);
		Ball->Velocity = glm::normalize(Ball->Velocity)*OLength;
		Ball->Stuck = Ball->Sticky;

		SoundEngine->play2D("bleep.wav");
	}

	for (auto& PowerUp : this->PowerUps)
	{
		if (!PowerUp.Destroyed)
		{
			if (PowerUp.Position.y >= this->Height)
			{
				PowerUp.Destroyed = true;
			}
			if (CheckCollision(*Player, PowerUp))
			{
				this->ActivatePowerUp(PowerUp);
				PowerUp.Destroyed = true;
				PowerUp.Activated = true;
				SoundEngine->play2D("powerup.wav");
			}

		}
	}
}

void FGame::ResetLevel()
{
	switch (this->Level)
	{
	case 0:this->Levels[0].Load("LevelOne.txt", this->Width, this->Height); break;
	case 1:this->Levels[1].Load("LevelTwo.txt", this->Width, this->Height); break;
	case 2:this->Levels[2].Load("LevelThree.txt", this->Width, this->Height); break;
	case 3:this->Levels[3].Load("LevelFour.txt", this->Width, this->Height); break;
	}
	this->Lives = 3;
}

void FGame::ResetPlayer()
{
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -Ball->Radius * 2), INITIAL_BALL_VELOCITY);
}

void FGame::SpawnPowerUps(FGameObject& Block)
{
	if (this->ShouldSpawn(75))
	{
		this->PowerUps.push_back(FPowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, Block.Position, FResourceManager::GetTexture("power_speed")));
	}
	if (this->ShouldSpawn(75))
	{
		this->PowerUps.push_back(FPowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, Block.Position, FResourceManager::GetTexture("power_sticky")));
	}
	if (this->ShouldSpawn(75))
	{
		this->PowerUps.push_back(FPowerUp("pass-through", glm::vec3(0.5f, 1.0f, 1.0f), 10.0f, Block.Position, FResourceManager::GetTexture("power_pass")));
	}
	if (this->ShouldSpawn(75))
	{
		this->PowerUps.push_back(FPowerUp("pas-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, Block.Position, FResourceManager::GetTexture("power_size")));
	}
	if (this->ShouldSpawn(15))
	{
		this->PowerUps.push_back(FPowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, Block.Position, FResourceManager::GetTexture("power_confuse")));
	}
	if (this->ShouldSpawn(15))
	{
		this->PowerUps.push_back(FPowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, Block.Position, FResourceManager::GetTexture("power_chaos")));
	}
}

void FGame::UpdatePowerUps(GLfloat DeltaTime)
{
	for (auto& PowerUp : this->PowerUps)
	{
		PowerUp.Position += PowerUp.Velocity*DeltaTime;
		if (PowerUp.Activated)
		{
			PowerUp.Duration -= DeltaTime;
			//cout << PowerUp.Duration << endl;
			if (PowerUp.Duration <= 0.0f)
			{
				PowerUp.Activated = false;
				if (PowerUp.Type == "sticky")
				{
					if (!IsOtherPowerUpActivate("sticky"))
					{
						Ball->Sticky = false;
						Player->Color = glm::vec3(1);
					}
				}
				else if (PowerUp.Type == "pass-through")
				{
					if (!IsOtherPowerUpActivate("pass-through"))
					{
						Ball->PassThrough = false;
						Ball->Color = glm::vec3(1);
					}
				}
				else if (PowerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActivate("confuse"))
					{
						PostProcessor->Confuse = false;
					}
				}
				else if (PowerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActivate("chaos"))
					{
						PostProcessor->Chaos = false;
					}
				}
			}
		}
	}
	this->PowerUps.erase(remove_if(this->PowerUps.begin(),this->PowerUps.end(),
		[](const FPowerUp& PowerUp)
		{
			return PowerUp.Destroyed && !PowerUp.Activated;
		}), this->PowerUps.end());
}

FCollision FGame::CheckCollision(FBallObject & A, FGameObject & B)
{
	glm::vec2 Center(A.Position + A.Radius);
	glm::vec2 AABBHalfExtents = B.Size / 2.0f;
	glm::vec2 AABBCenter = B.Position + AABBHalfExtents;
	glm::vec2 Difference = Center - AABBCenter;
	glm::vec2 Clamped = glm::clamp(Difference, -AABBHalfExtents, AABBHalfExtents);
	glm::vec2 Closest = AABBCenter + Clamped;

	Difference = Closest - Center;

	if (glm::length(Difference) <= A.Radius)
		return make_tuple(GL_TRUE, VectorDirection(Difference), Difference);
	else
		return make_tuple(GL_FALSE, UP, glm::vec2(0));

}

bool FGame::CheckCollision(FGameObject & A, FGameObject & B)
{
	bool CollisionX = (A.Position.x + A.Size.x >= B.Position.x) && (B.Position.x + B.Size.x >= A.Position.x);
	bool CollisionY = (A.Position.y + A.Size.y >= B.Position.y) && (B.Position.y + B.Size.y >= A.Position.y);

	return CollisionX && CollisionY;
}

glm::vec2 Compass[] = {
glm::vec2(0.0f, 1.0f),  // ио
glm::vec2(1.0f, 0.0f),  // ср
glm::vec2(0.0f, -1.0f), // об
glm::vec2(-1.0f, 0.0f)  // вС
};

Direction FGame::VectorDirection(glm::vec2 Target)
{
	float Max = 0.0f;
	int BestMatch = -1;
	for (int i = 0; i < 4; i++)
	{
		float Dot = glm::dot(glm::normalize(Target), Compass[i]);
		if (Dot > Max)
		{
			Max = Dot;
			BestMatch = i;
		}
	}
	return (Direction)BestMatch;
}

GLboolean FGame::ShouldSpawn(GLuint Chance)
{
	GLuint Random = rand() % Chance;
	return Random == 0;
}

void FGame::ActivatePowerUp(FPowerUp& PowerUp)
{
	if (PowerUp.Activated)
		return;
	PowerUp.Activated = true;
	if (PowerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2f;
	}
	else if (PowerUp.Type == "sticky")
	{
		Ball->Sticky = true;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (PowerUp.Type == "pass-through")
	{
		Ball->PassThrough = true;
		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (PowerUp.Type == "pas-size-increase")
	{
		Player->Size.x += 50;
	}
	else if (PowerUp.Type == "confuse")
	{
		if (!PostProcessor->Chaos)
		{
			PostProcessor->Confuse = true;
			cout << (int)PostProcessor->Confuse << endl;
		}
	}
	else if (PowerUp.Type == "chaos")
	{
		if (!PostProcessor->Confuse)
			PostProcessor->Chaos = true;
	}
}

bool FGame::IsOtherPowerUpActivate(const string & Type)
{
	for (auto& PowerUp : this->PowerUps)
	{
		if (PowerUp.Activated&&PowerUp.Type == Type)
			return true;
	}
	return false;
}

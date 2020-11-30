#include "FGame.h"

FGame::FGame(GLuint Width, GLuint Height)
{
	State = GAME_ACTIVE;
	this->Width = Width;
	this->Height = Height;
}

FGame::~FGame()
{

}
// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const GLfloat PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;
FGameObject* Player;
FSprite* Renderer;
FBallObject* Ball;

void FGame::Init()
{
	FResourceManager::LoadShader("sprite.vert", "sprite.frag", "", "sprite");
	glm::mat4 Projection = glm::ortho(0.0f, (GLfloat)this->Width, (GLfloat)this->Height, 0.0f, -1.0f, 1.0f);
	FResourceManager::GetShader("sprite").Use().SetInt("Image", 0);
	FResourceManager::GetShader("sprite").SetMat4("Projection", Projection);

	Renderer = new FSprite(FResourceManager::GetShader("sprite"));
	FResourceManager::LoadTexture("awesomeface.png", "face");
	FResourceManager::LoadTexture("background.jpg", "background");
	FResourceManager::LoadTexture("block.png", "block");
	FResourceManager::LoadTexture("block_solid.png", "blockSolid");
	FResourceManager::LoadTexture("paddle.png", "paddle");

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
}

void FGame::Update(GLfloat DeltaTime)
{
	Ball->Move(DeltaTime, this->Width);
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
}

void FGame::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		
		Renderer->Draw(FResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		this->Levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);
		Ball->Draw(*Renderer);
		cout << glGetError() << endl;
	}
	
}
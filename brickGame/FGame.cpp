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

FSprite* Renderer;

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

}

void FGame::Update(GLfloat DeltaTime)
{

}

void FGame::ProcessInput(GLfloat DeltaTime)
{

}

void FGame::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		Renderer->Draw(FResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		this->Levels[this->Level].Draw(*Renderer);
	}
	
}
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

}

void FGame::Update(GLfloat DeltaTime)
{

}

void FGame::ProcessInput(GLfloat DeltaTime)
{

}

void FGame::Render()
{
	Renderer->Draw(FResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	cout << glGetError() << endl;
}
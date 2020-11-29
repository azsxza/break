#pragma once
#include <glad/glad.h>
#include "FResourceManager.h"
#include "FSprite.h"

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class FGame
{
public:
	GameState State;
	GLboolean bKeys[1024];
	GLuint Width, Height;

	FGame(GLuint Width, GLuint Height);
	~FGame();

	void Init();
	void ProcessInput(GLfloat DeltaTime);
	void Update(GLfloat DeltaTime);
	void Render();

};
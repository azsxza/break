#pragma once
#include <glad/glad.h>
#include "FResourceManager.h"
#include "FSprite.h"
#include "FGameLevel.h"
#include "FBallObject.h"
#include <GLFW/glfw3.h>

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction 
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> FCollision;

class FGame
{
public:

	GameState State;
	GLboolean bKeys[1024];
	GLuint Width, Height;

	vector<FGameLevel> Levels;
	GLuint Level;

	FGame(GLuint Width, GLuint Height);
	~FGame();

	void Init();
	void ProcessInput(GLfloat DeltaTime);
	void Update(GLfloat DeltaTime);
	void Render();

	void DoCollision();

	void ResetLevel();
	void ResetPlayer();

private:
	FCollision CheckCollision(FBallObject& A, FGameObject& B);
	Direction VectorDirection(glm::vec2 Target);
};
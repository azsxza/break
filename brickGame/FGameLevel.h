#pragma once
#include <vector>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include "FGameObject.h"
#include "FSprite.h"
#include "FResourceManager.h"

using namespace std;

class FGameLevel
{
public:
	vector<FGameObject> Bricks;
	

	FGameLevel() {}
	void Load(const string& FileName, GLuint LevelWidth, GLuint LevelHeight);

	void Draw(FSprite& Sprite);
	GLboolean IsCompleted();
private:
	void Init(vector<vector<GLuint>> TitleData, GLuint LevelWidth, GLuint LevelHeight);

};
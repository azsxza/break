#include "FGameLevel.h"

//TODO Re
void FGameLevel::Load(const string & FileName, GLuint LevelWidth, GLuint LevelHeight)
{
	this->Bricks.clear();

	GLuint TileCode;
	string Line;
	ifstream Fstream(FileName);
	vector<vector<GLuint>> TileData;
	if (Fstream)
	{
		while (getline(Fstream, Line))
		{
			istringstream Sstream(Line);
			vector<GLuint> row;
			while (Sstream >> TileCode)
				row.push_back(TileCode);
			TileData.push_back(row);
		}
		if (TileData.size() > 0)
			this->Init(TileData, LevelWidth, LevelHeight);
	}
}

void FGameLevel::Draw(FSprite & Sprite)
{
	for (auto& Tile : this->Bricks)
	{
		if (!Tile.Destroyed)
			Tile.Draw(Sprite);
	}
}

GLboolean FGameLevel::IsCompleted()
{
	for (auto& Tile : this->Bricks)
	{
		if (!Tile.Destroyed && !Tile.IsSolid)
			return GL_FALSE;
	}
	return GL_TRUE;
}

void FGameLevel::Init(vector<vector<GLuint>> TileData, GLuint LevelWidth, GLuint LevelHeight)
{
	GLuint Height = TileData.size();
	GLuint Width = TileData[0].size();
	GLfloat UnitWidth = (GLfloat)LevelWidth / (GLfloat)Width;
	GLfloat UnitHeight = (GLfloat)LevelHeight / (GLfloat)Height;

	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			glm::vec2 Pos(UnitWidth*x, UnitHeight*y);
			glm::vec2 Size(UnitWidth, UnitHeight);
			if (TileData[y][x] == 1)
			{
				FGameObject Obj(Pos, Size, FResourceManager::GetTexture("blockSolid"), glm::vec3(0.8f, 0.8f, 0.7f));
				Obj.IsSolid = true;
				this->Bricks.push_back(Obj);
			}
			else if (TileData[y][x] > 1)
			{
				glm::vec3 Color(1.0f);
				switch (TileData[y][x])
				{
				case 2:Color = glm::vec3(0.2f, 0.6f, 1.0f); break;
				case 3:Color = glm::vec3(0.0f, 0.7f, 0.0f); break;
				case 4:Color = glm::vec3(0.8f, 0.8f, 0.4f); break;
				case 5:Color = glm::vec3(1.0f, 0.5f, 0.0f); break;
				}
				FGameObject Obj(Pos, Size, FResourceManager::GetTexture("block"), Color);
				this->Bricks.push_back(Obj);
			}

		}
	}
}

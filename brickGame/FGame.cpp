#include "FGame.h"

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;
FGameObject* Player;
FSprite* Renderer;
FBallObject* Ball;
FPaticleGenerator* Particle;
FPostProcesser* PostProcessor;

FGame::FGame(GLuint Width, GLuint Height)
{
	State = GAME_ACTIVE;
	this->Width = Width;
	this->Height = Height;
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

	Renderer = new FSprite(FResourceManager::GetShader("sprite"));
	Particle = new FPaticleGenerator(FResourceManager::GetShader("particle"), FResourceManager::GetTexture("particle"), 500);
	PostProcessor = new FPostProcesser(FResourceManager::GetShader("postprocess"), this->Width, this->Height);

	PostProcessor->Chaos = true;
	PostProcessor->Shake = true;

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
	this->DoCollision();
	Particle->Update(DeltaTime, *Ball, 2, glm::vec2(Ball->Radius / 2));
	if (Ball->Position.y >= this->Height)
	{
		this->ResetPlayer();
	}
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
		PostProcessor->BeginRender();
		Renderer->Draw(FResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		this->Levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);
		Particle->Draw();
		Ball->Draw(*Renderer);
		PostProcessor->EndRender();
		PostProcessor->Render(glfwGetTime());
		
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
				}
				Direction Dir = get<1>(Collision);
				glm::vec2 DiffVector = get<2>(Collision);
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
}

void FGame::ResetPlayer()
{
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -Ball->Radius * 2), INITIAL_BALL_VELOCITY);
}

FCollision FGame::CheckCollision(FBallObject & A, FGameObject & B)
{
	//bool CollisionX = (A.Position.x + A.Size.x >= B.Position.x) && (B.Position.x + B.Size.x >= A.Position.x);
	//bool CollisionY = (A.Position.y + A.Size.y >= B.Position.y) && (B.Position.y + B.Size.y >= A.Position.y);

	//return CollisionX && CollisionY;
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

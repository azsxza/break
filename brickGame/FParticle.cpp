#include "FParticle.h"

FParticle::FParticle()
{
	this->Position = glm::vec2(0);
	this->Velocity = glm::vec2(0);
	this->Color = glm::vec4(0);
	this->Life = 0;
}

FPaticleGenerator::FPaticleGenerator(FShader Shader, FTexture Texture, GLuint Amount)
{
	this->Shader = Shader;
	this->Texture = Texture;
	this->Amount = Amount;
	this->Init();
}

void FPaticleGenerator::Update(GLfloat DeltaTime, FGameObject & Object, GLuint NewParticles, glm::vec2 Offset)
{
	for (int i = 0; i < NewParticles; i++)
	{
		this->RespwnParticle(this->Particles[this->FirstUnusedParticle()], Object, Offset);
	}
	for (int i = 0; i < this->Amount; i++)
	{
		FParticle& P = this->Particles[i];
		P.Life -= DeltaTime;
		if (P.Life > 0)
		{
			P.Position -= P.Velocity*DeltaTime;
			P.Color.a -= DeltaTime * 2.5;
		}
	}
}

void FPaticleGenerator::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->Shader.Use();
	for (FParticle Particle : this->Particles)
	{
		this->Shader.SetVec2("offset", Particle.Position);
		this->Shader.SetVec4("color", Particle.Color);
		this->Texture.Active(GL_TEXTURE0);
		glBindVertexArray(this->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void FPaticleGenerator::Init()
{
	GLuint VBO;
	GLfloat ParticleQuad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleQuad), ParticleQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	for (int i = 0; i < this->Amount; i++)
	{
		this->Particles.push_back(FParticle());
	}
}

GLuint LastUsedParticle = 0;
GLuint FPaticleGenerator::FirstUnusedParticle()
{
	for (int i = LastUsedParticle; i < this->Amount; i++)
	{
		if (this->Particles[i].Life <= 0)
		{
			LastUsedParticle = i;
			return i;
		}
	}
	for (int i = 0; i < LastUsedParticle; i++)
	{
		if (this->Particles[i].Life <= 0)
		{
			LastUsedParticle = i;
			return i;
		}
	}
	LastUsedParticle = 0;
	return 0;
}

void FPaticleGenerator::RespwnParticle(FParticle & Particle, FGameObject & Object, glm::vec2 Offset)
{
	GLfloat Random = ((rand() % 100) - 50) / 10.0f;
	GLfloat RColor = 0.5 + ((rand() % 100) / 100.0f);
	Particle.Position = Object.Position + Random + Offset;
	Particle.Color = glm::vec4(glm::vec3(RColor), 1.0);
	Particle.Life = 1.0f;
	Particle.Velocity = Object.Velocity*0.1f;
}

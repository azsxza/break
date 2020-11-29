#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <iostream>

using namespace std;

class FHelper
{
public:
	char InfoLog[512];
	int Success;

	string GetShaderSource(string ShaderName);
	unsigned int GetShader(const char* ShaderSource, unsigned int ShaderClass);
	unsigned int GetShaderProgram(unsigned int VertexShader, unsigned int FragmentShader);
	unsigned int GetShaderProgram(unsigned int VertexShader, unsigned int FragmentShader, unsigned int GeometryShader);
};

class FShader
{
public:
	unsigned int ID;
	FHelper Helper;

	FShader() {};
	FShader(string VertexShaderFile, string FragmentShaderFile, string GeometryShaderFile = "");
	void Use();

	void SetBool(const string& Name, bool V);
	void SetInt(const string& Name, int V);
	void SetInt(int Loc, int V);
	void SetFloat(const string& Name, float V);
	void SetFloat(int Loc, float V);
	void SetVec2(const string& Name, float V1, float V2);
	void SetVec2(const string& Name, float V);
	void SetVec2(const string& Name, glm::vec2 V);
	void SetVec2(int Loc, float V1, float V2);
	void SetVec2(int Loc, float V);
	void SetVec2(int Loc, glm::vec2 V);
	void SetVec3(const string& Name, float V1, float V2, float V3);
	void SetVec3(const string& Name, float V);
	void SetVec3(const string& Name, glm::vec3 V);
	void SetVec3(int Loc, float V1, float V2, float V3);
	void SetVec3(int Loc, float V);
	void SetVec3(int Loc, glm::vec3 V);
	void SetVec4(const string& Name, float V1, float V2, float V3, float V4);
	void SetVec4(const string& Name, float V);
	void SetVec4(const string& Name, glm::vec4 V);
	void SetVec4(int Loc, float V1, float V2, float V3, float V4);
	void SetVec4(int Loc, float V);
	void SetVec4(int Loc, glm::vec4 V);
	void SetMat4(const string& Name, glm::mat4 V);
	void SetMat4(int Loc, glm::mat4 V);

	int GetUniformLocation(const string& Name);
};
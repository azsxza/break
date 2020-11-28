#pragma once
#include <unordered_map>
#include <string>
#include "FShader.h"
#include "FTexture.h"

using namespace std;

class FResourceManager
{
public:
	static unordered_map<string, FShader> Shaders;
	static unordered_map<string, FTexture> Textures;

	static FShader LoadShader(const string& VertexShaderFile, const string& FragmentShaderFile, const string& GeometryShaderFile = "", const string& Name="");
	static FShader GetShader(const string& Name);

	static FTexture LoadTexture(const string& ImageFile, const string& Name);
	static FTexture GetTexture(const string& Name);

	static void Clear();

private:
	FResourceManager();
};
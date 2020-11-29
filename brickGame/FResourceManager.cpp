#include "FResourceManager.h"

unordered_map<string, FShader> FResourceManager::Shaders;
unordered_map<string, FTexture> FResourceManager::Textures;

FShader FResourceManager::LoadShader(const string & VertexShaderFile, const string & FragmentShaderFile, const string & GeometryShaderFile, const string & Name)
{
	Shaders[Name] = FShader(VertexShaderFile, FragmentShaderFile, GeometryShaderFile);
	return Shaders[Name];
}

FShader& FResourceManager::GetShader(const string & Name)
{
	return Shaders[Name];
}

FTexture FResourceManager::LoadTexture(const string & ImageFile, const string & Name)
{
	Textures[Name] = FTexture(ImageFile);
	return Textures[Name];
}

FTexture& FResourceManager::GetTexture(const string & Name)
{
	return Textures[Name];
}

void FResourceManager::Clear()
{
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

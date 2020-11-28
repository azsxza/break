#include "FShader.h"
#include <fstream>


string FHelper::GetShaderSource(string shaderName)
{
	ifstream ShaderFile(shaderName);
	string Source = "";
	string Tmp;
	while (getline(ShaderFile, Tmp))
	{
		Source += Tmp + '\n';
	}
	ShaderFile.close();
	Source += '\0';
	return Source;
}

unsigned int FHelper::GetShader(const char* ShaderSource, unsigned int ShaderClass)
{
	unsigned int Shader = glCreateShader(ShaderClass);
	glShaderSource(Shader, 1, &ShaderSource, NULL);
	glCompileShader(Shader);
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
	if (!Success)
	{
		glGetShaderInfoLog(Shader, 512, NULL, InfoLog);
		std::cout << "+ERROR::SHADER::COMPILATION_FAILED\n" << InfoLog << std::endl;
	}
	return Shader;
}

unsigned int FHelper::GetShaderProgram(unsigned int VertexShader, unsigned int FragmentShader)
{
	int ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << std::endl;
	}

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	return ShaderProgram;
}

unsigned int FHelper::GetShaderProgram(unsigned int VertexShader, unsigned int FragmentShader, unsigned int GeometryShader)
{
	int ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glAttachShader(ShaderProgram, GeometryShader);
	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << std::endl;
	}

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	return ShaderProgram;
}

FShader::FShader(string VertexShaderFile, string FragmentShaderFile, string GeometryShaderFile)
{
	string VertexShaderSourceString = Helper.GetShaderSource(VertexShaderFile);
	string FragmentShaderSourceString = Helper.GetShaderSource(FragmentShaderFile);
	auto VertexShaderSource = VertexShaderSourceString.c_str();
	auto FragmentShaderSource = FragmentShaderSourceString.c_str();
	unsigned int VertexShader = Helper.GetShader(VertexShaderSource, GL_VERTEX_SHADER);
	unsigned int FragmentShader = Helper.GetShader(FragmentShaderSource, GL_FRAGMENT_SHADER);

	if (GeometryShaderFile != "")
	{
		string GeometryShaderSourceString = Helper.GetShaderSource(GeometryShaderFile);
		const char* GeometryShaderSource = GeometryShaderSourceString.c_str();
		unsigned int GeometryShader = Helper.GetShader(GeometryShaderSource, GL_GEOMETRY_SHADER);
		ID = Helper.GetShaderProgram(VertexShader, FragmentShader, GeometryShader);
	}
	else
		ID = Helper.GetShaderProgram(VertexShader, FragmentShader);
}

void FShader::use()
{
	glUseProgram(ID);
}

void FShader::SetBool(const string& Name, bool V)
{
	SetInt(Name, V);
}

void FShader::SetInt(const string& Name, int V)
{
	glUniform1i(glGetUniformLocation(ID, Name.c_str()), V);
}

void FShader::SetFloat(const string& Name, float V)
{
	glUniform1f(glGetUniformLocation(ID, Name.c_str()), V);
}

void FShader::SetVec2(const string& Name, float V1, float V2)
{
	glUniform2f(glGetUniformLocation(ID, Name.c_str()), V1, V2);
}

void FShader::SetVec2(const string& Name, float V)
{
	glUniform2f(glGetUniformLocation(ID, Name.c_str()), V, V);
}

void FShader::SetVec2(const string& Name, glm::vec2 V)
{
	glUniform2f(glGetUniformLocation(ID, Name.c_str()), V.x, V.y);
}

void FShader::SetVec3(const string& Name, float V1, float V2, float V3)
{
	glUniform3f(glGetUniformLocation(ID, Name.c_str()), V1, V2, V3);
}

void FShader::SetVec3(const string& Name, float V)
{
	SetVec3(Name, V, V, V);
}

void FShader::SetVec3(const string& Name, glm::vec3 V)
{
	SetVec3(Name, V.x, V.y, V.z);
}

void FShader::SetVec4(const string& Name, float V1, float V2, float V3, float V4)
{
	glUniform4f(glGetUniformLocation(ID, Name.c_str()), V1, V2, V3, V4);
}

void FShader::SetVec4(const string& Name, float V)
{
	SetVec4(Name, V, V, V, V);
}

void FShader::SetVec4(const string& Name, glm::vec4 V)
{
	SetVec4(Name, V.x, V.y, V.z, V.w);
}

void FShader::SetMat4(const string& Name, glm::mat4 V)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, glm::value_ptr(V));
}

void FShader::SetInt(int Loc, int V)
{
	glUniform1i(Loc, V);
}

void FShader::SetFloat(int Loc, float V)
{
	glUniform1f(Loc, V);
}

void FShader::SetVec2(int Loc, float V1, float V2)
{
	glUniform2f(Loc, V1, V2);
}

void FShader::SetVec2(int Loc, float V)
{
	glUniform2f(Loc, V, V);
}

void FShader::SetVec2(int Loc, glm::vec2 V)
{
	glUniform2f(Loc, V.x, V.y);
}

void FShader::SetVec3(int Loc, float V1, float V2, float V3)
{
	glUniform3f(Loc, V1, V2, V3);
}

void FShader::SetVec3(int Loc, float V)
{
	SetVec3(Loc, V, V, V);
}

void FShader::SetVec3(int Loc, glm::vec3 V)
{
	SetVec3(Loc, V.x, V.y, V.z);
}

void FShader::SetVec4(int Loc, float V1, float V2, float V3, float V4)
{
	glUniform4f(Loc, V1, V2, V3, V4);
}

void FShader::SetVec4(int Loc, float V)
{
	SetVec4(Loc, V, V, V, V);
}

void FShader::SetVec4(int Loc, glm::vec4 V)
{
	SetVec4(Loc, V.x, V.y, V.z, V.w);
}

void FShader::SetMat4(int Loc, glm::mat4 V)
{
	glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(V));
}

int FShader::GetUniformLocation(const string& Name)
{
	return glGetUniformLocation(ID, Name.c_str());
}

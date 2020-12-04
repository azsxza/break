#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stbi_image.h"
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "FGame.h"
#include "FResourceManager.h"



// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

FGame Breakout(SCR_WIDTH, SCR_HEIGHT);


void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mode)
{
	if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
		glfwSetWindowShouldClose(Window, GL_TRUE);
	if (Key >= 0 && Key < 1024)
	{
		if (Action == GLFW_PRESS)
			Breakout.bKeys[Key] = GL_TRUE;
		else if (Action == GLFW_RELEASE)
		{
			Breakout.bKeys[Key] = GL_FALSE;
			Breakout.bKeysProcessed[Key] = false;
		}
	}
}

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, KeyCallback);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Breakout.Init();
	GLfloat DeltaTime = 0.0f;
	GLfloat LastFrame = 0.0f;

	//Breakout.State = GAME_ACTIVE;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		GLfloat CurrentFrame = glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;
		glfwPollEvents();

		Breakout.ProcessInput(DeltaTime);

		Breakout.Update(DeltaTime);
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Breakout.Render();

		glfwSwapBuffers(window);
	}

	FResourceManager::Clear();
	glfwTerminate();
	return 0;
}


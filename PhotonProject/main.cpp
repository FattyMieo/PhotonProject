#include <GLFW/glfw3.h>
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <Windows.h>
#include <gl/GLU.h>

#include "FMODExtension.h"
#include "PhotonListener.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "MyApplication.h"

using namespace std;

//==================================================

#define RESOLUTION_X 800
#define RESOLUTION_Y 600

//==================================================

//FMOD
FMOD::System* fmodSystem = NULL;

//Photon
NetworkListener* networkListener = NULL;

//Engine
Application* app = new MyApplication();

//==================================================

//Delegates
void onWindowResized(GLFWwindow* window, int width, int height);
void onCursorMoved(GLFWwindow* window, double xPos, double yPos);
void onMouseButton(GLFWwindow* window, int button, int action, int mods);
void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);

//==================================================

void gameStart()
{
	networkListener->connect();
	app->Start();
}

void gameUpdate(float deltaTime)
{
	networkListener->run();
	app->Update(deltaTime);
}

void gameDraw()
{
	app->Draw();
}

int main()
{
	//--------------------------------------------------
	//  Variables
	//--------------------------------------------------

	//GLFW
	GLFWwindow* window;
	const char* windowTitle = "PhotonProject";
	bool useVSync = true;

	//Engine
	float lastUpdateTime = (float)glfwGetTime();

	//--------------------------------------------------

	//Set random seed
	srand((unsigned int)time(NULL));

	//Initialize the library
	if (!glfwInit())
	{
		return -1;
	}

	// Create m_a windowed mode window and its OpenGL context
	window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, windowTitle, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Enable / Disbale VSync
	glfwSwapInterval((useVSync ? 1 : 0));

	//Setup delegates
	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwMakeContextCurrent(window); //Make the window's context current
	onWindowResized(window, RESOLUTION_X, RESOLUTION_Y);

	glfwSetCursorPosCallback(window, onCursorMoved);
	glfwSetMouseButtonCallback(window, onMouseButton);
	glfwSetKeyCallback(window, onKey);

	// To support alpha channel
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);

	//Setup FMOD
	FMODExtension::FMOD_Init(fmodSystem);

	//Initialize the network
	networkListener = new NetworkListener();

	app->InitDependencies(fmodSystem, networkListener);

	// Run Application Start
	gameStart();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate delta time and FPS
		float deltaTime = (float)glfwGetTime() - lastUpdateTime;
		lastUpdateTime = (float)glfwGetTime();
		
		gameUpdate(deltaTime);
		gameDraw();

		glfwSwapBuffers(window);

		//This is for inputs to work!
		glfwPollEvents();
	}

	//Termination
	glfwTerminate();
	return 0;
}

//==================================================

void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Use ortho2D view
	gluOrtho2D(0, width, height, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void onCursorMoved(GLFWwindow* window, double xPos, double yPos)
{
	app->OnMouseCursorMove((float)xPos, (float)yPos);
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		app->OnMousePress(button);
		break;
	case GLFW_RELEASE:
		app->OnMouseRelease(button);
		break;
	}
}

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		app->OnKeyPress(key);
		break;
	case GLFW_RELEASE:
		app->OnKeyRelease(key);
		break;
	}
}

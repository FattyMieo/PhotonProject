#include <iostream>
#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <Windows.h>
#include <string>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>
#include "Drawer.h"
#include "MyPhoton.h"

//==================================================

#define RESOLUTION_X 800
#define RESOLUTION_Y 600

#define TEXTURE_COUNT 2

//==================================================

//Textures
enum TextureName
{
	T_Cursor = 0,
};
GLuint mTextureID[TEXTURE_COUNT];
unsigned int mTextureSize[TEXTURE_COUNT][2];

//Photon
MyPhoton* network = NULL;
const int ID = 0;
float cursorPos[2];

//==================================================

//Delegates
void onWindowResized(GLFWwindow* window, int width, int height);
void onCursorMoved(GLFWwindow* window, double xPos, double yPos);
void onMouseButton(GLFWwindow* window, int button, int action, int mods);

//==================================================

int main()
{
	//--------------------------------------------------
	//  Variables
	//--------------------------------------------------

	//GLFW
	GLFWwindow* window;
	const char* windowTitle = "PhotonProject";

	//--------------------------------------------------

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

	//Setup delegates
	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwMakeContextCurrent(window); //Make the window's context current
	onWindowResized(window, RESOLUTION_X, RESOLUTION_Y);

	glfwSetCursorPosCallback(window, onCursorMoved);

	glfwSetMouseButtonCallback(window, onMouseButton);

	// To support alpha channel
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);

	//Load Textures
	//Need to call after window's created
	glEnable(GL_TEXTURE_2D);
	glGenTextures(TEXTURE_COUNT, mTextureID);

	Drawer::LoadTexture("Cursor.png", mTextureID[T_Cursor], mTextureSize[T_Cursor][0], mTextureSize[T_Cursor][1]);

	//Initialize the network
	network = new MyPhoton();
	network->connect();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Drawer::DrawSquare
		(
			mTextureID[T_Cursor],
			(int)cursorPos[0],
			RESOLUTION_Y - (int)cursorPos[1],
			32,
			32,
			0
		);

		Drawer::DrawSquare
		(
			mTextureID[T_Cursor],
			(int)network->testCursor[0],
			RESOLUTION_Y - (int)network->testCursor[1],
			32,
			32,
			0
		);
		
		network->run();

		glfwSwapBuffers(window);
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
	gluOrtho2D(0, width, 0, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void onCursorMoved(GLFWwindow* window, double xPos, double yPos)
{
	cursorPos[0] = (float)xPos;
	cursorPos[1] = (float)yPos;

	if (network != NULL)
	{
		network->sendEvent(ID, cursorPos[0], cursorPos[1]);
		if (glfwGetMouseButton(window, GLFW_PRESS))
		{

		}
	}
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (network != NULL)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			network->sendEvent(ID, 1.0f, 0.0f);
		}
	}
}

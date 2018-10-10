//#include <iostream>
//#include "MyPhoton.h"
//#include <Windows.h>
//#include <GL/GLU.h>
//#include <GLFW/glfw3.h>
//
//const int RESOLUTION_X = 800;
//const int RESOLUTION_Y = 600;
//
//MyPhoton* network = NULL;
//
//void onWindowResized(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	//! Use ortho2D view
//	gluOrtho2D(0, width, 0, height);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
//{
//	if (network != NULL)
//	{
//		if (glfwGetMouseButton(window, GLFW_PRESS))
//		{
//			int myID = 2;
//			network->sendEvent(myID, xPos, yPos);
//		}
//	}
//}
//
//static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//	if (network != NULL)
//	{
//		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//		{
//			int myID = 2;
//			network->sendEvent(myID, 1.0f, 0.0f);
//		}
//	}
//}
//
//
//int main(void)
//{
//	GLFWwindow* window;
//
//	/* Initialize the library */
//	if (!glfwInit())
//		return -1;
//
//	/* Create m_a windowed mode window and its OpenGL context */
//	window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "", NULL, NULL);
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwSetWindowSizeCallback(window, onWindowResized);
//
//	/* Make the window's context current */
//	glfwMakeContextCurrent(window);
//
//	onWindowResized(window, RESOLUTION_X, RESOLUTION_Y);
//	glfwSetCursorPosCallback(window, cursor_position_callback);
//	glfwSetMouseButtonCallback(window, mouse_button_callback);
//
//	//Initialize the network
//	network = new MyPhoton();
//	network->connect();
//	while (!glfwWindowShouldClose(window))
//	{
//		network->run();
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//	return 0;
//}
#pragma once

#include <GLFW/glfw3.h>
#ifndef GL_EXTENSION_H
#include "GLExtension.h"
#endif
#include "lodepng.h"
#include "Vector2.h"

class Drawer
{
public:
	static void LoadPNG(const char* path, GLuint textureID, unsigned int& width, unsigned int& height);
	static void LoadTexture(const char* fileName, GLuint textureID, unsigned int& width, unsigned int& height);
	static void DrawGeometry(GLuint textureID, GLfloat* vertices, GLfloat* texCoords, GLubyte* colors);

	static void DrawSquare(GLuint textureID, int x, int y, int width, int height, int depth, GLubyte r, GLubyte g, GLubyte b, GLubyte a, Vector2 minUV, Vector2 maxUV);
	static void DrawSquare(GLuint textureID, int x, int y, int width, int height, int depth, GLubyte r, GLubyte g, GLubyte b, GLubyte a);
	static void DrawSquare(GLuint textureID, int x, int y, int width, int height, int depth);
};
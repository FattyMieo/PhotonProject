#pragma once

#include <GLFW/glfw3.h>
#include "MathExtension.h"

class Color
{
public:
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;

	Color();
	Color(GLubyte c);
	Color(GLubyte c, GLubyte a);
	Color(GLubyte r, GLubyte g, GLubyte b);
	Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a);

	static Color Random(Color a, Color b);
	static Color Lerp(Color a, Color b, float t);
};
#include "Color.h"

Color::Color()
{
	r = 255;
	g = 255;
	b = 255;
	a = 255;
}

Color::Color(GLubyte c)
{
	this->r = c;
	this->g = c;
	this->b = c;
	this->a = 255;
}

Color::Color(GLubyte c, GLubyte a)
{
	this->r = c;
	this->g = c;
	this->b = c;
	this->a = a;
}

Color::Color(GLubyte r, GLubyte g, GLubyte b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255;
}

Color::Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color Color::Random(Color darker, Color lighter)
{
	Color ret;
	ret.r = (GLubyte)MathExtension::GetRandomInt(darker.r, lighter.r);
	ret.g = (GLubyte)MathExtension::GetRandomInt(darker.g, lighter.g);
	ret.b = (GLubyte)MathExtension::GetRandomInt(darker.b, lighter.b);
	ret.a = (GLubyte)MathExtension::GetRandomInt(darker.a, lighter.a);

	return ret;
}

Color Color::Lerp(Color a, Color b, float t)
{
	Color ret;
	ret.r = (GLubyte)MathExtension::Lerp(a.r, b.r, t);
	ret.g = (GLubyte)MathExtension::Lerp(a.g, b.g, t);
	ret.b = (GLubyte)MathExtension::Lerp(a.b, b.b, t);
	ret.a = (GLubyte)MathExtension::Lerp(a.a, b.a, t);

	return ret;
}

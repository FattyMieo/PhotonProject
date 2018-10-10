#include "Drawer.h"
#include <iostream>

const char* pathToMedia = "../Media/";

void Drawer::LoadPNG(const char* path, GLuint textureID, unsigned int& width, unsigned int& height)
{
	//Load file and decode image
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, width, height, path);

	//If there's an error, display it
	if (error != 0)
	{
		std::cout << "png load error :" << error << ": " << lodepng_error_text(error) << std::endl;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);

	//Repeat the texture after exceeding 1.0f
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // apply texture wrapping along horizontal part
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // apply texture wrapping along vertical part

	//Uses last pixel after exceeding 1.0f
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // apply texture wrapping along horizontal part
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // apply texture wrapping along vertical part

	// bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // near filtering (For when texture needs to scale...)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // far filtering (For when texture needs to scale...)

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // near filtering (For when texture needs to scale...)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // far filtering (For when texture needs to scale...)

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
}

void Drawer::LoadTexture(const char* fileName, GLuint textureID, unsigned int& width, unsigned int& height)
{
	std::string absolutePath = "";
	absolutePath += pathToMedia;
	absolutePath += fileName;
	return LoadPNG(absolutePath.c_str(), textureID, width, height);
}

void Drawer::DrawGeometry(GLuint textureID, GLfloat* vertices, GLfloat* texCoords, GLubyte* colors)
{
	glEnable(GL_TEXTURE_2D); //Enable texturing
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_TEXTURE_2D); //Disable texturing
}

void Drawer::DrawSquare(GLuint textureID, int x, int y, int width, int height, int depth, GLubyte r, GLubyte g, GLubyte b, GLubyte a, Vector2 minUV, Vector2 maxUV)
{
	float halfWidth = (float)width / 2.0f;
	float halfHeight = (float)height / 2.0f;

	float depth_f = depth;

	GLfloat vertices[] =
	{
		x - halfWidth, y - halfHeight, depth_f,
		x - halfWidth, y + halfHeight, depth_f,
		x + halfWidth, y - halfHeight, depth_f,

		x - halfWidth, y + halfHeight, depth_f,
		x + halfWidth, y - halfHeight, depth_f,
		x + halfWidth, y + halfHeight, depth_f
	};

	GLfloat* texCoords = GLExtension::GetUV(minUV, maxUV);

	GLubyte colors[] =
	{
		r, g, b, a,
		r, g, b, a,
		r, g, b, a,

		r, g, b, a,
		r, g, b, a,
		r, g, b, a
	};

	DrawGeometry(textureID, vertices, texCoords, colors);
}

void Drawer::DrawSquare(GLuint textureID, int x, int y, int width, int height, int depth, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	DrawSquare(textureID, x, y, width, height, depth, r, g, b, a, Vector2(0.0, 1.0), Vector2(1.0, 0.0));
}

void Drawer::DrawSquare(GLuint textureID, int x, int y, int width, int height, int depth)
{
	DrawSquare(textureID, x, y, width, height, depth, 255, 255, 255, 255);
}
#include "Sprite.h"

Sprite::Sprite()
{
	m_width = 128;
	m_height = 128;

	SetBlendingMode(BM_Alpha);
	SetRenderOrder(0);
	SetUV(Vector2(0.0f), Vector2(1.0f));
	hFlip = false;
}

Sprite::Sprite(const std::string& file)
{
	LoadFromFile(file);

	SetBlendingMode(BM_Alpha);
	SetRenderOrder(0);
	SetUV(Vector2(0.0f), Vector2(1.0f));
	hFlip = false;
}

Sprite::~Sprite()
{

}

void Sprite::LoadFromFile(const std::string& file)
{
	glGenTextures(1, m_textureID);
	Drawer::LoadTexture(file.c_str(), m_textureID[0], m_width, m_height);
}

void Sprite::SetTextureID(const GLuint textureID)
{
	this->m_textureID[0] = textureID;
}

void Sprite::SetUV(const Vector2 min, const Vector2 max)
{
	this->m_minUV = min;
	this->m_maxUV = max;
}

Vector2 Sprite::GetDimension()
{
	return Vector2((float)m_width, (float)m_height);
}

void Sprite::SetDimension(const int x, const int y)
{
	m_width = x;
	m_height = y;
}

void Sprite::SetColor(const Color& color)
{
	m_color = color;
}

const Color& Sprite::GetColor()
{
	return m_color;
}

void Sprite::SetBlendingMode(BlendMode mode)
{
	m_blendMode = mode;
}

BlendMode Sprite::GetBlendingMode()
{
	return m_blendMode;
}

void Sprite::SetRenderOrder(int order)
{
	m_renderOrder = order;
}

int Sprite::GetRenderOrder()
{
	return m_renderOrder;
}

void Sprite::Draw()
{
	Matrix transMatrix = Matrix::makeTranslationMatrix(Vector(m_transform.position.x, m_transform.position.y, 0.0f));
	Matrix rotMatrix = Matrix::makeRotateMatrix(m_transform.rotation, Vector(0.0f, 0.0f, 1.0f));
	Matrix scaleMatrix = Matrix::makeScaleMatrix(Vector(m_transform.scale.x, m_transform.scale.y, 0.0f));
	Matrix hFlipMatrix = Matrix::makeHorizontalFlipMatrix();

	Matrix viewMatrix = transMatrix * rotMatrix * scaleMatrix;
	if (hFlip) viewMatrix = viewMatrix * hFlipMatrix;

	glLoadMatrixf((GLfloat*)viewMatrix.mVal);
	
	// To support alpha channel
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

	glDepthMask(true); //Enable the depth mask

	switch (GetBlendingMode())
	{
		case BM_Additive:
			//glBlendFunc(GL_ONE, GL_ONE); //No anti-alias
			glBlendFunc(GL_SRC_ALPHA, GL_ONE); //With anti-alias (alpha value)
			break;
		case BM_Multiply:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			//glBlendFunc(GL_DST_COLOR, GL_ZERO);
			glDepthMask(false); //Ignore the depth mask, for semi-transparent objects
			break;
		case BM_Alpha:
		default:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
	}

	Drawer::DrawSquare(m_textureID[0], 0, 0, m_width, m_height, m_renderOrder, GetColor().r, GetColor().g, GetColor().b, GetColor().a, m_minUV, m_maxUV);

	glDepthMask(true); //Reenable the depth mask

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
}

void Sprite::Draw(float x, float y, float rotation, float scaleX, float scaleY)
{
	m_transform.position.x = x;
	m_transform.position.y = y;
	m_transform.rotation = rotation;
	m_transform.scale.x = scaleX;
	m_transform.scale.y = scaleY;

	Draw();
}

void Sprite::Draw(Vector2 position, float rotation, Vector2 scale)
{
	m_transform.position = position;
	m_transform.rotation = rotation;
	m_transform.scale = scale;

	Draw();
}

void Sprite::Draw(Transform2D newTransform)
{
	m_transform = newTransform;

	Draw();
}
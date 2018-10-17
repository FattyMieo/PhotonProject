#include "Spritesheet.h"

Spritesheet::Spritesheet()
{
	m_spritesheet = new Sprite();

	m_hCount = 0;
	m_vCount = 0;
}

Spritesheet::Spritesheet(const std::string& file, int hCount, int vCount)
{
	m_spritesheet = new Sprite(file);

	m_hCount = hCount;
	m_vCount = vCount;
}

Spritesheet::~Spritesheet()
{
	delete m_spritesheet;
}

int Spritesheet::GetTotalSprites()
{
	return m_hCount * m_vCount;
}

Sprite Spritesheet::GetSprite(unsigned int i)
{
	return GetSprite(i, BM_Alpha);
}

Sprite Spritesheet::GetSprite(unsigned int i, Sprite copySprite)
{
	return GetSprite(i % m_hCount, i / m_vCount, copySprite);
}

Sprite Spritesheet::GetSprite(unsigned int x, unsigned int y)
{
	//Wrap the parameters around the obtainable range
	x = x % m_hCount;
	y = y % m_vCount;

	//Make a copy of the spritesheet
	Sprite ret = *m_spritesheet;

	//Change the UV
	ret.SetUV
	(
		Vector2((float)(x / (float)m_hCount), (float)(y / (float)m_vCount)),
		Vector2((float)((x + 1) / (float)m_hCount), (float)((y + 1) / (float)m_vCount))
	);

	Vector2 dimension = ret.GetDimension();

	ret.SetDimension((int)(dimension.x / m_hCount), (int)(dimension.y / m_vCount));

	return ret;
}

Sprite Spritesheet::GetSprite(unsigned int x, unsigned int y, Sprite copySprite)
{
	Sprite ret = GetSprite(x, y);

	ret.SetColor(copySprite.GetColor());
	ret.SetBlendingMode(copySprite.GetBlendingMode());

	return ret;
}

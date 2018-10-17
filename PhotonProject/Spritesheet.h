#pragma once
#include "Vector2.h"
#include "Sprite.h"

class Spritesheet
{
private:
	Sprite* m_spritesheet;

	int m_hCount;
	int m_vCount;

public:
	Spritesheet();
	Spritesheet(const std::string& file, int hCount, int vCount);
	virtual ~Spritesheet();
	int GetTotalSprites();

	Sprite GetSprite(unsigned int i);
	Sprite GetSprite(unsigned int i, Sprite copySprite);
	Sprite GetSprite(unsigned int x, unsigned int y);
	Sprite GetSprite(unsigned int x, unsigned int y, Sprite copySprite);
};

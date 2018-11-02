#pragma once
#include "GameObject.h"
#include "Spritesheet.h"
#include <string>

class Text : public GameObject
{
protected:
	Spritesheet textSS = Spritesheet("SimpleMood_16x16_ASCII.png", 16, 16);
	std::string contentText;
	int contentLength = 0;
	Sprite* contentArray;

public:

	Text();
	Text(const std::string text);
	virtual ~Text();

	void Draw() override;

	void SetText(const std::string text);
	std::string GetText();
};
#include "Text.h"

Text::Text() : GameObject() { }

Text::Text(const std::string text) : GameObject()
{
	SetText(text);
}

Text::~Text()
{

}

void Text::Draw()
{
	Transform2D trans = GetTransform();
	float textWidth = textSS.GetDimension().x * trans.scale.x;
	trans.position.x -= textWidth * (((float)contentLength / 2.0f) + 0.5f);

	for (int i = 0; i < contentLength; ++i)
	{
		trans.position.x += textWidth;
		contentArray[i].Draw(trans);
	}
}

void Text::SetText(const std::string text)
{
	contentText = text;

	contentLength = text.length();

	contentArray = new Sprite[contentLength];

	for (int i = 0; i < contentLength; ++i)
	{
		contentArray[i] = textSS.GetSprite((unsigned int)contentText[i]);
	}
}

std::string Text::GetText()
{
	return contentText;
}

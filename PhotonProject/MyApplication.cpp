#include "MyApplication.h"
#include <iostream>

MyApplication::MyApplication() : Application()
{

}

MyApplication::~MyApplication()
{

}

void MyApplication::Start()
{
	Application::Start();

	Sprite* sp;

	myCursor = Instantiate(Vector2(0.0f), 0.0f);
	sp = &(myCursor->GetSprite());
	sp->LoadFromFile("Cursor.png");
	sp->SetDimension(32, 32);
	sp->SetBlendingMode(BM_Alpha);

	otherCursor = Instantiate(Vector2(0.0f), 0.0f);
	sp = &(otherCursor->GetSprite());
	sp->LoadFromFile("Cursor.png");
	sp->SetDimension(32, 32);
	sp->SetBlendingMode(BM_Alpha);
}

void MyApplication::Update(float deltaTime)
{
	Application::Update(deltaTime);

	Vector2 pos = otherCursor->GetTransform().position;
	otherCursor->GetTransform().position = Vector2
	(
		MathExtension::Lerp(pos.x, otherCursorPos.x, 0.1f),
		MathExtension::Lerp(pos.y, otherCursorPos.y, 0.1f)
	);
}

void MyApplication::OnMouseCursorMove(float x, float y)
{
	myCursor->GetTransform().position = Vector2(x, y);

	if (m_networkListener != NULL)
	{
		m_networkListener->sendEvent(0, x, y);
	}
}

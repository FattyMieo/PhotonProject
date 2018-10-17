#pragma once

#include "Application.h"
#include "Spritesheet.h"

class MyApplication : public Application
{
public:
	//Temporary names
	GameObject* myCursor;
	GameObject* otherCursor;
	Vector2 otherCursorPos;

	MyApplication();
	virtual ~MyApplication();

	void Start() override;
	void Update(float deltaTime) override;
	void OnMouseCursorMove(float x, float y);
};
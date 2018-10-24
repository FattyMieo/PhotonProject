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

	static MyApplication* GetInstance()
	{
		static MyApplication app;
		return &app;
	}

	void Start() override;
	void Update(float deltaTime) override;
	void OnMouseCursorMove(float x, float y);

	void SendMove(float x, float y);

	virtual void OnReceiveNetworkEvent(byte* packedData, uint size);
	virtual void SendNetworkEvent(byte* packedData, uint size);
};
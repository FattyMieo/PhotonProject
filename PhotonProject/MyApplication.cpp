#include "MyApplication.h"
#include <iostream>
#include "DataPacker.h"
#include "DataSplitter.h"
#include "ResDefinition.h"

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

	otherCursor->SetActive(false);
}

void MyApplication::Update(float deltaTime)
{
	Application::Update(deltaTime);

	if (timer < maxTime)
	{
		timer += deltaTime;
	}
	else if (needsUpdate)
	{
		needsUpdate = false;
		timer -= maxTime;
		SendMove(myCursor->GetTransform().position.x, myCursor->GetTransform().position.y);
	}

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
	needsUpdate = true;
	//SendMove(x, y);
}

void MyApplication::SendMove(float x, float y)
{
	uint px, py;
	px = (uint)x;
	py = (uint)y;
	DataPacker<uint> packer;
	packer.Pack(px, sizeof(ushort) * 8);
	packer.Pack(py, sizeof(ushort) * 8);

	DataSplitter<uint> splitter;
	splitter.Split(packer.GetData());

	SendNetworkEvent(splitter.GetArray(), sizeof(uint));
}

void MyApplication::OnReceiveNetworkEvent(byte* packedData, uint size)
{
	DataSplitter<uint> splitter;
	splitter.SetArray(packedData);

	DataPacker<uint> packer;
	packer.SetData(splitter.Merge());
	uint ex, ey;
	packer.Extract(ey, sizeof(ushort) * 8);
	packer.Extract(ex, sizeof(ushort) * 8);
	otherCursorPos.x = (float)ex;
	otherCursorPos.y = (float)ey;
}

void MyApplication::OnJoinRoomEvent(int playerID)
{
	otherCursor->SetActive(true);
	SendMove(myCursor->GetTransform().position.x, myCursor->GetTransform().position.y);
}

void MyApplication::OnLeaveRoomEvent(int playerID)
{
	otherCursor->SetActive(false);
}

void MyApplication::SendNetworkEvent(byte* packedData, uint size)
{
	if (m_networkListener != NULL)
	{
		m_networkListener->sendEvent(packedData, size);
	}
}

#include "MyApplication.h"
#include <iostream>
#include <sstream>
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
	std::stringstream ss;

	statusText = new Text("Initializing...");
	m_GameObjects.GetList().push_back(statusText);
	statusText->Start();
	statusText->GetTransform().position = Vector2(RESOLUTION_X / 2.0f, 64.0f);
	
	boardSprite = Instantiate(Vector2(RESOLUTION_X / 2.0f, RESOLUTION_Y / 2.0f), 0.0f);
	sp = &(boardSprite->GetSprite());
	sp->LoadFromFile("Board.png");
	sp->SetDimension(256, 256);
	sp->SetBlendingMode(BM_Alpha);

	boardSymbols = new Text*[9];
	for (int i = 0; i < 9; ++i)
	{
		boardSymbols[i] = new Text(" ");
		m_GameObjects.GetList().push_back(boardSymbols[i]);
		boardSymbols[i]->Start();
		float posX = ((i % 3) - 1) * 96.0f;
		float posY = ((i / 3) - 1) * 96.0f;
		boardSymbols[i]->GetTransform().position = Vector2(RESOLUTION_X / 2.0f + posX, RESOLUTION_Y / 2.0f + posY);
		boardSymbols[i]->GetTransform().scale = Vector2(4.0f);
		boardSymbols[i]->SetText("#");
	}

	myCursor = Instantiate(Vector2(0.0f), 0.0f);
	sp = &(myCursor->GetSprite());
	sp->LoadFromFile("Cursor.png");
	sp->SetDimension(128, 128);
	sp->SetBlendingMode(BM_Alpha);
	sp->SetColor(Color(128, 255, 255));

	otherCursor = Instantiate(Vector2(0.0f), 0.0f);
	sp = &(otherCursor->GetSprite());
	sp->LoadFromFile("Cursor.png");
	sp->SetDimension(128, 128);
	sp->SetBlendingMode(BM_Alpha);
	sp->SetColor(Color(255, 128, 128));

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

void MyApplication::OnMousePress(int button)
{
	for (int i = 0; i < 9; ++i)
	{
		if
		(
			myCursor->GetTransform().position.x >=
			boardSymbols[i]->GetTransform().position.x -
			(
				boardSymbols[i]->GetSprite().GetDimension().x / 2.0f
				- 32.0f
			)
			&&
			myCursor->GetTransform().position.x <=
			boardSymbols[i]->GetTransform().position.x +
			(
				boardSymbols[i]->GetSprite().GetDimension().x / 2.0f
				- 32.0f
			)
			&&
			myCursor->GetTransform().position.y >=
			boardSymbols[i]->GetTransform().position.y -
			(
				boardSymbols[i]->GetSprite().GetDimension().y / 2.0f
				- 32.0f
			)
			&&
			myCursor->GetTransform().position.y <=
			boardSymbols[i]->GetTransform().position.y +
			(
				boardSymbols[i]->GetSprite().GetDimension().y / 2.0f
				- 32.0f
			)
		)
		{
			std::cout << "Pressed: " << i << std::endl;

			if (localPlayerID == activePlayer)
			{
				board.owned[i] = (EPlayerType)(activePlayer - 1);
				SendCommand(i);
				activePlayer = (activePlayer % 2) + 1;
			}
		}
	}
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

void MyApplication::SendCommand(int boardCell)
{
	DataPacker<byte> packer;
	packer.Pack(boardCell, 8);

	DataSplitter<byte> splitter;
	splitter.Split(packer.GetData());

	SendNetworkEvent(splitter.GetArray(), sizeof(byte));
}

void MyApplication::UpdateStatusText()
{
	std::stringstream ss;
	//ss.str(""); //Clears the stream
	switch (board.status)
	{
	case EStatus::EStatus_Waiting:
		ss << "Waiting for opponent...";
		break;
	case EStatus::EStatus_Playing:
		ss << "Now is Your Turn!";
		break;
	case EStatus::EStatus_End:
		switch (board.endStatus)
		{
		case EEndStatus::EEndStatus_Victory:
			ss << char(1) << " Victorious " << char(1);
			break;
		case EEndStatus::EEndStatus_Defeat:
			ss << char(2) << "  Defeated  " << char(2);
			break;
		case EEndStatus::EEndStatus_Draw:
			ss << char(247) << "    Draw    " << char(247);
			break;
		}
		break;
	}

	statusText->SetText(ss.str());
}

void MyApplication::UpdateBoard()
{
	for (int i = 0; i < 9; ++i)
	{
		if (board.owned[i] == EPlayerType::EPlayerType_O)
		{
			boardSymbols[i]->SetText("O");
		}
		else if (board.owned[i] == EPlayerType::EPlayerType_X)
		{
			boardSymbols[i]->SetText("X");
		}
		else
		{
			boardSymbols[i]->SetText(" ");
		}
	}
}

bool MyApplication::CheckForWinCondition(EPlayerType playerCell)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if
			(
				board.owned[i * 3 + j] != playerCell
			)
			continue;

			if (j >= 2) return true;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if
			(
				board.owned[j * 3 + i] != playerCell
			)
			continue;

			if (j >= 2) return true;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		if
		(
			board.owned[i * 3 + i] != playerCell
		)
		break;

		if (i >= 2) return true;
	}

	for (int i = 0; i < 3; ++i)
	{
		if
		(
			board.owned[(2 - i) * 3 + (2 - i)] != playerCell
		)
		break;

		if (i >= 2) return true;
	}

	return false;
}

void MyApplication::OnReceiveNetworkEvent(byte* packedData, uint size)
{
	if (size == sizeof(uint))
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
	else if (size == sizeof(byte))
	{
		DataSplitter<byte> splitter;
		splitter.SetArray(packedData);

		DataPacker<byte> packer;
		packer.SetData(splitter.Merge());

		byte bCommand;
		packer.Extract(bCommand, 8);

		uint command = (uint)bCommand;
		if (command >= 0 && command <= 8)
		{
			board.owned[command] = (EPlayerType)(activePlayer - 1);
			activePlayer = (activePlayer % 2) + 1;
		}
	}

	UpdateStatusText();
	UpdateBoard();
	if (CheckForWinCondition((EPlayerType)(activePlayer - 1)))
	{
		std::cout << "GG" << std::endl;
		board.status = EStatus::EStatus_End;
		if (localPlayerID == activePlayer)
			board.endStatus = EEndStatus::EEndStatus_Victory;
		else
			board.endStatus = EEndStatus::EEndStatus_Defeat;
	}

	if (board.status != EStatus::EStatus_End)
	{
		if (localPlayerID == activePlayer)
		{
			board.status = EStatus::EStatus_Playing;
		}
		else
		{
			board.status = EStatus::EStatus_Waiting;
		}
	}
}

void MyApplication::OnCreateRoomEvent(int playerID)
{
	if (localPlayerID < 0)
	{
		localPlayerID = playerID;
		isRoomOwner = true;
		std::cout << localPlayerID << std::endl;
	}
}

void MyApplication::OnJoinRoomEvent(int playerID)
{
	if (localPlayerID < 0)
	{
		localPlayerID = playerID;
		std::cout << localPlayerID << std::endl;
	}

	if (playerID > 1)
	{
		otherCursor->SetActive(true);
		SendMove(myCursor->GetTransform().position.x, myCursor->GetTransform().position.y);
		UpdateBoard();

		activePlayer = 1;
		if (localPlayerID == activePlayer)
		{
			board.status = EStatus::EStatus_Playing;
		}
		else
		{
			board.status = EStatus::EStatus_Waiting;
		}
	}

	UpdateStatusText();

	if (CheckForWinCondition((EPlayerType)(activePlayer - 1)))
	{
		std::cout << "GG" << std::endl;
		board.status = EStatus::EStatus_End;
		if (localPlayerID == activePlayer)
			board.endStatus = EEndStatus::EEndStatus_Victory;
		else
			board.endStatus = EEndStatus::EEndStatus_Defeat;
	}
}

void MyApplication::OnLeaveRoomEvent(int playerID)
{
	if (localPlayerID != playerID)
	{
		otherCursor->SetActive(false);
	}
}

void MyApplication::SendNetworkEvent(byte* packedData, uint size)
{
	if (m_networkListener != NULL)
	{
		m_networkListener->sendEvent(packedData, size);
	}
}

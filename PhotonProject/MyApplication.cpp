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
	}

	functionButton = new Text("");
	m_GameObjects.GetList().push_back(functionButton);
	functionButton->Start();
	functionButton->GetTransform().position = Vector2(RESOLUTION_X / 2.0f, RESOLUTION_Y - 64.0f);

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
		SendMove(myCursor->GetTransform().position);
	}

	otherCursor->GetTransform().position =
		Vector2::Lerp
		(
			otherCursor->GetTransform().position,
			otherCursorPos,
			0.1f
		);
}

void MyApplication::PreUpdateGame()
{
	if (CheckForDrawCondition())
	{
		board.status = EStatus::EStatus_End;
		board.endStatus = EEndStatus::EEndStatus_Draw;
	}
	else if (CheckForWinCondition((EPlayerType)(activePlayer - 1))) // Minus one (PlayerID starts on 1)
	{
		board.status = EStatus::EStatus_End;
		if (localPlayerID == activePlayer)
			board.endStatus = EEndStatus::EEndStatus_Victory;
		else
			board.endStatus = EEndStatus::EEndStatus_Defeat;
	}
}

void MyApplication::PostUpdateGame()
{
	if (board.status != EStatus::EStatus_End)
	{
		if (localPlayerID == activePlayer)
			board.status = EStatus::EStatus_Playing;
		else
			board.status = EStatus::EStatus_Waiting;
	}

	UpdateStatusText();
}

void MyApplication::UpdateGame()
{
	PreUpdateGame();
	GoToNextPlayer(activePlayer);
	PostUpdateGame();
}

void MyApplication::UpdateStatusText()
{
	std::stringstream ss;
	std::stringstream ssBTN;
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
		case EEndStatus::EEndStatus_SVictory:
			ss << char(1) << " Opponent Surrendered " << char(1);
			break;
		case EEndStatus::EEndStatus_SDefeat:
			ss << char(2) << "     Surrendered     " << char(2);
			break;
		}
		break;
	}

	if (activePlayer > 0)
	{
		if (board.status != EStatus::EStatus_End)
		{
			ssBTN << "Surrender";
		}
		else
		{
			ssBTN << "Rematch";
		}
	}

	statusText->SetText(ss.str());
	functionButton->SetText(ssBTN.str());
}

void MyApplication::UpdateCell(int cell)
{
	if (board.owned[cell] == EPlayerType::EPlayerType_O)
	{
		boardSymbols[cell]->SetText("O");
	}
	else if (board.owned[cell] == EPlayerType::EPlayerType_X)
	{
		boardSymbols[cell]->SetText("X");
	}
	else
	{
		boardSymbols[cell]->SetText(" ");
	}
}

void MyApplication::UpdateBoard()
{
	for (int i = 0; i < 9; ++i)
	{
		UpdateCell(i);
	}
}

void MyApplication::RestartGame(int startingPlayer)
{
	for (int i = 0; i < 9; ++i)
	{
		activePlayer = startingPlayer;
		board.owned[i] = EPlayerType::EPlayerType_Empty;
		UpdateCell(i);
	}

	board.status = EStatus::EStatus_Waiting;
	PostUpdateGame();
}

bool MyApplication::CheckCursorWithinButton(Vector2 curPos)
{
	if
	(
		curPos.x >=
		functionButton->GetTransform().position.x -
		functionButton->GetText().length() / 4.0f * 
		(
			functionButton->GetSprite().GetDimension().x / 2.0f
			- 32.0f
		)
		&&
		curPos.x <=
		functionButton->GetTransform().position.x +
		functionButton->GetText().length() / 4.0f *
		(
			functionButton->GetSprite().GetDimension().x / 2.0f
			- 32.0f
		)
		&&
		curPos.y >=
		functionButton->GetTransform().position.y -
		(
			functionButton->GetSprite().GetDimension().y / 2.0f
			- 32.0f
		)
		&&
		curPos.y <=
		functionButton->GetTransform().position.y +
		(
			functionButton->GetSprite().GetDimension().y / 2.0f
			- 32.0f
		)
	)
	{
		return true;
	}

	return false;
}

int MyApplication::CheckCursorOnCell(Vector2 curPos)
{
	for (int i = 0; i < 9; ++i)
	{
		if
		(
			curPos.x >=
			boardSymbols[i]->GetTransform().position.x -
			(
				boardSymbols[i]->GetSprite().GetDimension().x / 2.0f
				- 32.0f
			)
			&&
			curPos.x <=
			boardSymbols[i]->GetTransform().position.x +
			(
				boardSymbols[i]->GetSprite().GetDimension().x / 2.0f
				- 32.0f
			)
			&&
			curPos.y >=
			boardSymbols[i]->GetTransform().position.y -
			(
				boardSymbols[i]->GetSprite().GetDimension().y / 2.0f
				- 32.0f
			)
			&&
			curPos.y <=
			boardSymbols[i]->GetTransform().position.y +
			(
				boardSymbols[i]->GetSprite().GetDimension().y / 2.0f
				- 32.0f
			)
		)
		{
			return i;
		}
	}

	return -1;
}

bool MyApplication::CheckForDrawCondition()
{
	for (int i = 0; i < 9; ++i)
	{
		if (board.owned[i] == EPlayerType::EPlayerType_Empty)
			return false;
	}
	return true;
}

bool MyApplication::CheckForWinCondition(EPlayerType playerCell)
{
	for (int i = 0; i <= 2; ++i)
	{
		for (int x = 0; x <= 2; ++x)
		{
			if (board.owned[i * 3 + x] != playerCell) break;
			if (x >= 2) return true;
		}

		for (int y = 0; y <= 2; ++y)
		{
			if (board.owned[y * 3 + i] != playerCell) break;
			if (y >= 2) return true;
		}
	}

	for (int i = 0; i <= 2; ++i)
	{
		if (board.owned[i * 3 + i] != playerCell) break;
		if (i >= 2) return true;
	}

	for (int i = 2; i >= 0; --i)
	{
		if (board.owned[i * 3 + i] != playerCell) break;
		if (i <= 0) return true;
	}

	return false;
}

EPlayerType MyApplication::CheckForWinCondition()
{
	if (CheckForWinCondition(EPlayerType::EPlayerType_O)) return EPlayerType::EPlayerType_O;
	if (CheckForWinCondition(EPlayerType::EPlayerType_X)) return EPlayerType::EPlayerType_X;
	return EPlayerType::EPlayerType_Empty;
}

void MyApplication::GoToNextPlayer(int & curPlayer)
{
	curPlayer = (curPlayer % 2) + 1;
	// Index | PlayerID | (activePlayer % 2) | (activePlayer % 2) + 1
	//-------|----------|--------------------|------------------------
	//   0   |    1     |         1          |           2
	//   1   |    2     |         0          |           1
}

void MyApplication::OnMouseCursorMove(float x, float y)
{
	myCursor->GetTransform().position = Vector2(x, y);
	needsUpdate = true; //SendMove(x, y);
}

void MyApplication::OnMousePress(int button)
{
	if (CheckCursorWithinButton(myCursor->GetTransform().position))
	{
		if (activePlayer > 0)
		{
			if (board.status != EStatus::EStatus_End)
			{
				//Surrender
				board.status = EStatus::EStatus_End;
				board.endStatus = EEndStatus::EEndStatus_SDefeat;
				UpdateStatusText();

				SendCommand(10);
			}
			else
			{
				//Rematch (Opponent starts first)
				RestartGame((localPlayerID % 2) + 1);

				SendCommand(10);
			}
		}
	}
	else
	{
		// If the game is not ended...
		if (board.status != EStatus::EStatus_End)
		{
			// If it's my turn...
			if (localPlayerID == activePlayer)
			{
				// Find the cell I click
				int selectedCell = CheckCursorOnCell(myCursor->GetTransform().position);

				// If it's a valid cell...
				if (selectedCell >= 0)
				{
					std::cout << "Pressed: " << selectedCell << std::endl;

					// If the cell is empty...
					if (board.owned[selectedCell] == EPlayerType::EPlayerType_Empty)
					{
						// Set last choice
						lastChoiceIndex = selectedCell;
						lastChoicePlayerType = board.owned[selectedCell];

						// Set cell to mine
						board.owned[selectedCell] = (EPlayerType)(activePlayer - 1); // Minus one (PlayerID starts on 1)
						UpdateCell(selectedCell);

						// Tell opponent the move
						SendCommand(selectedCell);

						// Update the game
						UpdateGame();
					}
				}
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

void MyApplication::SendMove(Vector2 cursorPos)
{
	return SendMove(cursorPos.x, cursorPos.y);
}

void MyApplication::SendCommand(int boardCell)
{
	DataPacker<byte> packer;
	packer.Pack(boardCell, 8);

	DataSplitter<byte> splitter;
	splitter.Split(packer.GetData());

	SendNetworkEvent(splitter.GetArray(), sizeof(byte));
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
			std::cout << "Received: " << command << std::endl;

			// If game is not ended AND the cell is empty...
			if 
			(
				board.status != EStatus::EStatus_End &&
				board.owned[command] == EPlayerType::EPlayerType_Empty
			)
			{
				// Set cell to opponent's
				board.owned[command] = (EPlayerType)(activePlayer - 1); // Minus one (PlayerID starts on 1)
				UpdateCell(command);

				// Update the game
				UpdateGame();
			}
			else
			{
				// Tell opponent the move is invalid
				SendCommand(9);
			}
		}
		else
		{
			if (command == 9)
			{
				board.owned[lastChoiceIndex] = lastChoicePlayerType;
				UpdateCell(lastChoiceIndex);

				UpdateGame();
				statusText->SetText("Invalid move!");
			}
			else if (command == 10)
			{
				if (activePlayer > 0)
				{
					if (board.status != EStatus::EStatus_End)
					{
						//Opponent Surrender
						board.status = EStatus::EStatus_End;
						board.endStatus = EEndStatus::EEndStatus_SVictory;
						UpdateStatusText();
					}
					else
					{
						//Rematch
						RestartGame(localPlayerID);
					}
				}
			}
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
			board.status = EStatus::EStatus_Playing;
		else
			board.status = EStatus::EStatus_Waiting;
	}

	UpdateStatusText();
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

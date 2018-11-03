#pragma once

#include "Application.h"
#include "Spritesheet.h"
#include "Text.h"
#include "BoardState.h"

class MyApplication : public Application
{
public:
	//Temporary names
	GameObject* myCursor;
	GameObject* otherCursor;
	Vector2 otherCursorPos;
	float maxTime = 0.1f;
	float timer = 0.0f;
	bool needsUpdate = false;

	int localPlayerID = -1;
	int activePlayer = -1;
	bool isRoomOwner = false;

	Text* statusText;
	BoardState board;
	GameObject* boardSprite;
	Text** boardSymbols;
	Text* functionButton;

	int lastChoiceIndex = -1;
	EPlayerType lastChoicePlayerType = EPlayerType::EPlayerType_Empty;

	MyApplication();
	virtual ~MyApplication();

	static MyApplication* GetInstance()
	{
		static MyApplication app;
		return &app;
	}

	void Start() override;
	void Update(float deltaTime) override;

	void PreUpdateGame();
	void PostUpdateGame();
	void UpdateGame();
	void UpdateStatusText();
	void UpdateCell(int cell);
	void UpdateBoard();
	void RestartGame(int startingPlayer);

	bool CheckCursorWithinButton(Vector2 curPos);
	int CheckCursorOnCell(Vector2 curPos);
	bool CheckForDrawCondition();
	bool CheckForWinCondition(EPlayerType playerCell);
	EPlayerType CheckForWinCondition();
	void GoToNextPlayer(int& curPlayer);

	void OnMouseCursorMove(float x, float y);
	void OnMousePress(int button);
	void SendMove(float x, float y);
	void SendMove(Vector2 cursorPos);
	void SendCommand(int boardCell);
	virtual void OnReceiveNetworkEvent(byte* packedData, uint size);
	virtual void OnCreateRoomEvent(int playerID);
	virtual void OnJoinRoomEvent(int playerID);
	virtual void OnLeaveRoomEvent(int playerID);
	virtual void SendNetworkEvent(byte* packedData, uint size);
};
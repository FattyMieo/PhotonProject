#pragma once
#include "TypeDefinition.h"

enum EStatus
{
	EStatus_Waiting = 0,
	EStatus_Playing,
	EStatus_End
};

enum EEndStatus
{
	EEndStatus_Victory = 0,
	EEndStatus_Defeat,
	EEndStatus_Draw,
	EEndStatus_SVictory,
	EEndStatus_SDefeat,
};

enum EPlayerType
{
	EPlayerType_Empty = -1,
	EPlayerType_O = 0,
	EPlayerType_X = 1,
};

struct BoardState
{
	EStatus status;
	EEndStatus endStatus;
	EPlayerType owned[9];

	BoardState()
	{
		for (int i = 0; i < 9; ++i)
		{
			owned[i] = EPlayerType::EPlayerType_Empty;
		}
	}
};

struct BoardCommand
{
	uint selectedBox;
	// (4 bytes)
	//0 - 8 = MakeMove
	//9 = InvalidMove
	//10 = Rematch / Surrender
	//11 = 
	//12 = 
	//13 = 
	//14 = 
	//15 = 
};

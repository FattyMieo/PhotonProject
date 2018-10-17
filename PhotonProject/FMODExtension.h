#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include <stdio.h>
#include <iostream>
#include <string>

namespace FMODExtension
{
	//FMOD Error Check
	static void FMOD_ErrorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			//system("PAUSE");
		}
	}

	static void FMOD_Init(FMOD::System* m_fmodSystem)
	{
		FMOD_RESULT result;
		unsigned int version;

		result = FMOD::System_Create(&m_fmodSystem);
		FMOD_ErrorCheck(result);

		result = m_fmodSystem->getVersion(&version);
		FMOD_ErrorCheck(result);

		if (version < FMOD_VERSION)
		{
			printf("FMOD Error! You are using an old version of FMOD. Version: %d. FMOD_VERSION: %d\n", version, FMOD_VERSION);
		}

		//Initialise fmod system
		result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
		FMOD_ErrorCheck(result);
	}
}

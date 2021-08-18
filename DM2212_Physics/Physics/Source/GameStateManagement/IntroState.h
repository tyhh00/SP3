#pragma once

/**
 CIntroState
 @brief This class is derived from CGameStateBase. It will introduce the game to the player.
 By: Toh Da Jun
 Date: July 2021
 */

#include "GameStateBase.h"
#include "../SceneSplashScreen.h"

//#include "Primitives/Mesh.h"
//#include "../Scene2D/BackgroundEntity.h"
#include <string>

#include "../SoundController/SoundController.h"

class CIntroState : public CGameStateBase
{
public:
	// Constructor
	CIntroState(void);
	// Destructor
	~CIntroState(void);

	CSoundController* soundController;

	// Init this class instance
	virtual bool Init(void);
	// Update this class instance
	virtual bool Update(const double dElapsedTime);
	// Render this class instance
	virtual void Render(void);
	// Destroy this class instance
	virtual void Destroy(void);

protected:
	SceneSplashScreen* sceneSplash;
};

#pragma once
#include "SingletonTemplate.h"
#include "Portal.h"
#include "Dash.h"
#include "Grappling.h"
#include "Recall.h"
#include "SlowTime.h"
#include "BlackHole.h"


class GameManager : public CSingletonTemplate<GameManager>
{
	friend CSingletonTemplate<GameManager>;

public:

	enum GAMESTORY_STATE {
		GS_INTRO,
		GS_DEFAULT,
		GS_END,

		GS_TOTAL,
	};

	void Init();

	bool getMachineStatus(int partNum);
	void setMachineStatus(int partNum, bool obtained);

	GAMESTORY_STATE getGameState();
	void setGameState(GAMESTORY_STATE state);

	void initAbilities(SceneBase* scene, Camera* camera, GameObjectManager* GOM, GameObject* player);
	Ability* getCurrAbility(int abilityNum);
	void setAbility(int abilityNum, ABILITY_TYPE type);
	void removeAbility(int abilityNum);

	void addScore(float score);
	void removeScore(float score);
	void setScore(float score);

	float getScore();
	float getGameElapsed();

	void addCoins(int coins);
	void removeCoins(int coins);
	int getCoins();
	void setCoins(int coins);


protected:

	GameManager();
	~GameManager();


	float score;
	int coinsCollected;

	// GAME STORY STATE
	GAMESTORY_STATE game_state;
	// TIME MACHINE PARTS COLLECTION STATUS
	bool timeMachineStatus[4];
	// ABILITIES
	char ABILITY_KEYBIND_1;
	char ABILITY_KEYBIND_2;
	Ability* abilityArray[ABILITY_COUNT];
	Ability* currAbility[2];
	

	// TBC
	int abilitiesUsed;
	int collectedItems;
	float damageDealt;
	float damageReceived;
	double gameElapsed; //From load of lobby scene till end of game
};


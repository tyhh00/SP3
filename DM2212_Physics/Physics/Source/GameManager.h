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

	void Init();

	bool getMachineStatus(int partNum);
	void setMachineStatus(int partNum, bool obtained);

	void setScenePointers(SceneBase* scene, Camera* camera, GameObjectManager* GOM);
	Ability* getAbility(int abilityNum);
	void setAbility(int abilityNum, Ability* ability);

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

	bool timeMachineStatus[4];

	float score;
	int coinsCollected;


	// ABILITIES
	Ability* abilityArray[ABILITY_COUNT];

	Ability* currAbility[2];

	// TBC
	int abilitiesUsed;
	int collectedItems;
	float damageDealt;
	float damageReceived;
	double gameElapsed; //From load of lobby scene till end of game
};


#pragma once
#include "SingletonTemplate.h"

class GameManager : public CSingletonTemplate<GameManager>
{
	friend CSingletonTemplate<GameManager>;

public:

	void Init();

	bool getMachineStatus(int partNum);
	void setMachineStatus(int partNum, bool obtained);

	void addScore(float score);
	void removeScore(float score);
	void setScore(float score);

	float getScore();
	float getGameElapsed();

	void addCoins(int coins);
	void removeCoins(int coins);
	void setCoins(int coins);

	int getCoins();

protected:
	GameManager();
	~GameManager();

	bool timeMachineStatus[4];

	float score;
	int coinsCollected;


	// ABILITIES
	// creat ablity* for each

	// TBC
	int abilitiesUsed;
	int collectedItems;
	float damageDealt;
	float damageReceived;
	double gameElapsed; //From load of lobby scene till end of game
};


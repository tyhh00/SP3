#pragma once
#include "SingletonTemplate.h"

class GameManager : public CSingletonTemplate<GameManager>
{
	friend CSingletonTemplate<GameManager>;

public:

	bool getGameOverStatus();
	void setGameOver(bool);

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
	void Init();

	bool gameOver;
	double gameElapsed; //From load of lobby scene till end of game

	float score;
	int coinsCollected;

	//Todo
	int abilitiesUsed;
	int collectedItems;
	float damageDealt;
	float damageReceived;
};


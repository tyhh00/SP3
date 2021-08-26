#include "GameManager.h"
#include "MyMath.h"
#include "UIManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	gameOver = false;
	gameElapsed = 0.f;
	score = 0;
	coinsCollected = 0;
}

bool GameManager::getGameOverStatus()
{
	return gameOver;
}

void GameManager::setGameOver(bool gameOver)
{
	this->gameOver = gameOver;
}

void GameManager::addScore(float score)
{
	if (score > 0)
		this->score += score;
}

void GameManager::setScore(float score)
{
	this->score = score;
}

void GameManager::removeScore(float score)
{
	if (this->score -= score < 0)
		this->score = 0;
	this->score -= score;
}

float GameManager::getScore()
{
	return score;
}

void GameManager::addCoins(int coinsCollected)
{
	coinsCollected = abs(coinsCollected);
	this->coinsCollected += coinsCollected;

	//This UI is created in LobbyState
	UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("coinvalue")->setText(std::to_string(this->coinsCollected));
}

void GameManager::removeCoins(int coinsCollected)
{
	if (this->coinsCollected -= coinsCollected < 0)
		this->coinsCollected = 0;
	this->coinsCollected -= coinsCollected;

	//This UI is created in LobbyState
	UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("coinvalue")->setText(std::to_string(this->coinsCollected));
}

void GameManager::setCoins(int coinsCollected)
{
	this->coinsCollected = Math::Clamp(this->coinsCollected, 0, abs(coinsCollected));
	//This UI is created in LobbyState
	UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("coinvalue")->setText(std::to_string(this->coinsCollected));
}

int GameManager::getCoins()
{
	return this->coinsCollected;
}

float GameManager::getGameElapsed()
{
	return gameElapsed;
}


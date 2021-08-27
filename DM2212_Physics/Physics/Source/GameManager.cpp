#include "GameManager.h"
#include "MyMath.h"
#include "UIManager.h"

GameManager::GameManager()
{

}

GameManager::~GameManager()
{
	for (int i = 0; i < 2; i++)
	{
		if (currAbility[i])
		{
			delete currAbility[i];
			currAbility[i] = NULL;
		}
	}
	for (int i = 0; i < ABILITY_COUNT; i++)
	{
		if (abilityArray[i])
		{
			delete abilityArray[i];
			abilityArray[i] = NULL;
		}
	}
}

void GameManager::Init()
{
	gameElapsed = 0.f;
	score = 0;
	coinsCollected = 0;

	for (int i = 0; i < 4; i++)
	{
		timeMachineStatus[i] = false;
	}

	// create abilities
	
	Mesh* blackholeIcon = MeshBuilder::GenerateQuad("ability_blackhole", Color(1, 1, 1), 2.0f);
	blackholeIcon->textureID = LoadTGA("Image//black_hole.tga");
	Mesh* recallIcon = MeshBuilder::GenerateQuad("ability_recall", Color(1, 1, 1), 2.0f);
	recallIcon->textureID = LoadTGA("Image//recall_ability.tga");
	Mesh* portalIcon = MeshBuilder::GenerateQuad("ability_portal", Color(1, 1, 1), 2.0f);
	portalIcon->textureID = LoadTGA("Image//PortalAbilityIcon.tga");
	Mesh* grappleIcon = MeshBuilder::GenerateQuad("ability_grapple", Color(1, 1, 1), 2.0f);
	grappleIcon->textureID = LoadTGA("Image//Grappling.tga");
	Mesh* dashIcon = MeshBuilder::GenerateQuad("ability_dash", Color(1, 1, 1), 2.0f);
	dashIcon->textureID = LoadTGA("Image//Dash.tga");
	Mesh* slowtimeIcon = MeshBuilder::GenerateQuad("ability_slowtime", Color(1, 1, 1), 2.0f);
	slowtimeIcon->textureID = LoadTGA("Image//slow_time.tga");

	abilityArray[ABILITY_PORTAL] = new PortalAbility(portalIcon);
	abilityArray[ABILITY_DASH] = new DashAbility(dashIcon);
	abilityArray[ABILITY_BLACKHOLE] = new BlackHoleAbility(blackholeIcon);
	abilityArray[ABILITY_GRAPPLER] = new GrapplingAbility(grappleIcon);
	abilityArray[ABILITY_SLOWTIME] = new SlowTimeAbility(slowtimeIcon);
	abilityArray[ABILITY_RECALL] = new RecallAbility(recallIcon);
}

bool GameManager::getMachineStatus(int partNum)
{
	return timeMachineStatus[partNum - 1];
}

void GameManager::setMachineStatus(int partNum, bool obtained)
{
	timeMachineStatus[partNum - 1] = obtained;
}

void GameManager::setScenePointers(SceneBase* scene, Camera* camera, GameObjectManager* GOM)
{
	for (int i = 0; i < ABILITY_COUNT; i++)
	{
		abilityArray[i]->SetCamera(camera);
		abilityArray[i]->SetScenePointer(scene);
		abilityArray[i]->SetGOManager(GOM);
	}
}

Ability* GameManager::getAbility(int abilityNum)
{
	return currAbility[abilityNum - 1];
}

void GameManager::setAbility(int abilityNum, Ability* ability)
{
	currAbility[abilityNum - 1] = ability;
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


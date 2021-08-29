#ifndef SCENE_LOBBY_H
#define SCENE_LOBBY_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"
#include "Input.h"
#include "GameObjectManager.h"
#include "LobbyPortal.h"
#include "GameStateManagement/GameStateManager.h"
#include "SceneManager.h"
#include "Buttons/DialogueManager.h"
#include "GameManager.h"


class SceneLobby : public SceneBase
{
public:
	SceneManager *sceneManager;


	SceneLobby();
	~SceneLobby();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void InitLights();

protected:

	float m_speed;

	bool showMachinePartsUI;
	bool showAbilityUI;

	bool selectedAbilities = false;
	bool showEtoInteract = false;

	Player* player;
	GameObject* timeMachine, * abilityMachine, * wizardMachine;
	LobbyPortal* portal_graveyard, *portal_jungle, *portal_ocean, *portal_robot;

	Mesh* machinePartsUIBG, * machinePartsSlot;
	std::vector<Button*> machinePartsUIButtons;
	ButtonManager* buttonManager;

	Mesh* abilityUIBG, * grapplingAbilityUI, * dashAbilityUI, * portalAbilityUI, * recallAbilityUI, * slowTimeAbilityUI, * blackHoleAbilityUI;
	Mesh* ability1, * ability2, *clearAbility;
	std::vector<Button*> abilityUIButtons;

	Input* input;
	Inventory* inventory;
	GameObjectManager* goManager;
	DialogueManager* dialogueManager;
	GameManager* gameManager;
	Mesh* pressEtoInteract;

	double gridLength, gridHeight;
	void CursorToWorldPosition(double& theX, double& theY);
};

#endif
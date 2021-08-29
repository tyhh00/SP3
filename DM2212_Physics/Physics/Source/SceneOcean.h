#ifndef SCENE_OCEAN_H
#define SCENE_OCEAN_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"
#include "Input.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "Trident.h"
#include "Crab.h"
#include "Dragon.h"
#include "ThunderBlock.h"
#include "Whale.h"
#include "YH.h"
#include "Buttons/DialogueManager.h"

class SceneOcean : public SceneBase
{
public:
	SceneOcean();
	~SceneOcean();

	virtual void Init();
	virtual void Update(double dt);
	virtual void InitLights();
	virtual void Render();
	virtual void Exit();

protected:
	enum STORY_STATE {
		INTRO, // intro to the scene
		FINDWHALE, // player interact with whale, whale ask for requirements
		WHALE_TEXT, // if requirements are met, set state to FIND_YH. else, tell player requirements not met
		FIND_YH, // player interact with yh, yh ask for bottle
		YH_TEXT, // if bottle found, set state to OCEAN_END. else, tell player bottle not found
		OCEAN_END, // sets gameWin to true
		SS_TOTAL,
	};

	//STORY STATE
	STORY_STATE story_state;
	
	//PLAYER
	Player* player;
	
	//INVENTORY
	Inventory* inventory;
	
	//INPUT
	Input* input;
	
	//GAME OBJECT MANAGER
	GameObjectManager* goManager;
	
	//GAME MANAGER
	GameManager* gameManager;
	
	//DIALOGUE MANAGER
	DialogueManager* dialogueManager;
	
	//NPCS
	YongHong* yh;
	Whale* whale;

	//VARIABLES
	double gridLength, gridHeight;
	float m_speed;
};

#endif
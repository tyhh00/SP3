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
		INTRO, // if got tutorial then let player pick up flashlight and learn to use it,, can run dialogues too if any
		FINDWHALE, // wait for initial meeting w gatekeeper, add dialogue + give pickaxe
		WHALE_TEXT,
		GATEKEEPER2, // wait for church entry requirements
		FIND_YH,
		YH_TEXT,
		OCEAN_END,

		SS_TOTAL,
	};

	float m_speed;

	Player* player;
	Inventory* inventory;

	Input* input;
	GameObjectManager* goManager;
	GameManager* gameManager;
	DialogueManager* dialogueManager;

	double gridLength, gridHeight;

	YongHong* yh;
	Whale* whale;

	STORY_STATE story_state;
};

#endif
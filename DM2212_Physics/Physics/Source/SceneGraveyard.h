#ifndef SCENE_GRAVEYARD_H
#define SCENE_GRAVEYARD_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"
#include "Input.h"
#include "GameObjectManager.h"
#include "SceneManager.h"

class SceneGraveyard : public SceneBase
{
public:
	SceneGraveyard();
	~SceneGraveyard();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void InitLights();

protected:

	enum STORY_STATE {
		GY_INTRO, // if got tutorial then let player pick up flashlight and learn to use it,, can run dialogues too if any
		GY_DEFAULT, // main gameplay in graveyard scene
		GY_GATEKEEPER1, // initial meeting w gatekeeper, player given a pickaxe
		CHURCH_INTRO, // after meeting requirements, enter church, play any intro dialogue/storyline
		CHURCH_DEFAULT, // main boss fight gameplay
		CHURCH_END, // after defeating boss storyline/dialogue or anyth rlly

		SS_TOTAL,
	};

	float m_speed;

	STORY_STATE story_state;

	Inventory* inventory;
	Input* input;
	GameObjectManager* goManager;
	Player* player;
	
	double gridLength, gridHeight;
	void CursorToWorldPosition(double& theX, double& theY);
	void LoadBossScene();
};

#endif
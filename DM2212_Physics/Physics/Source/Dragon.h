#ifndef DRAGON_H
#define DRAGON_H

#include "GameObject.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "GameObjectManager.h"
//#include "Inventory.h"

class Dragon : public Enemy {
public:
	Dragon();
	~Dragon();

	void Init(SceneBase* scene, GameObject* target, int numParts, GameObjectManager* goManager); //init
	void Update(double dt); //update
	void Render(SceneBase* scene); //render
	float convertidk(float pain); //update angle

private:
	enum ENEMY_STATE {
		SINCURVE,
		GSTATE_TOTAL
	};
	//ENEMY STATE
	ENEMY_STATE state;

	//PLAYER
	GameObject* player; 
	
	//DRAGON HEAD
	GameObject* dragonHead;

	//GAME OBJECT MANAGER
	GameObjectManager* goM;

	//DRAGON GO VECTOR LIST
	std::vector<GameObject*> dragon;
	
	//VARIABLES
	float angle; 
	float curve;
	double curveTimer;
};


#endif
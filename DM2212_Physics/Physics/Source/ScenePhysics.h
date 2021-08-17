#ifndef SCENE_PHYSICS_H
#define SCENE_PHYSICS_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Apple.h"
#include "Cheese.h"
#include "Keyboard.h"
#include "GameObjectManager.h"

class ScenePhysics : public SceneBase
{
public:
	ScenePhysics();
	~ScenePhysics();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

protected:

	float m_speed;
	float m_worldWidth;
	float m_gameWidth;
	float m_worldHeight;

	GameObject* go;
	Inventory* inventory;
	Keyboard* keyboard;

	GameObject* FetchGO(bool isMovable);
	bool CheckCollision(GameObject* go1, GameObject* go2, float dt); // Checks for collision
	void ApplyFriction(GameObject* ball, Vector3 normal, double dt); // Apply Friction
	void ApplyInelastic(GameObject* ball, Vector3 normal, double dt); //Decrease vel of object
	void CollisionResponse(GameObject* go1, GameObject* go2, float dt);
	Keyboard kb;
	GameObjectManager* goManager;
};

#endif
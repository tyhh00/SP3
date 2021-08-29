#ifndef THUNDERBLOCK_H
#define THUNDERBLOCK_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Lightning.h"

class ThunderBlock : public GameObject {
public:
	ThunderBlock(GameObjectManager* goManager) //constructor that initialises the goManager
	{
		goM = goManager;
		lightning = nullptr;
	};
	~ThunderBlock()
	{};

	void Init()
	{
		this->type = GO_THUNDERBLOCK;
		theta = 0;
	};
	void Update(double dt)
	{
		//rotate block
		theta += 200 * dt; 
		physics->SetDir(Vector3(cos(Math::DegreeToRadian(theta)), sin(Math::DegreeToRadian(theta)), 0)); 
		physics->SetNormal(physics->GetDir().Cross(Vector3(0,0,-1)));
		
		//sets lightning to inactive after lightningTimer < 0
		if (lightning != nullptr)
		{
			if (lightning->getTimer() < 0)
			{
				lightning->dead = true;
				lightning = nullptr;
			}
		}
	};

	//spawns lightning
	void SpawnLightning()
	{
		lightning = new Lightning();
		lightning->physics = physics->Clone();
		lightning->Init(pos, physics->GetDir());
		lightning->active = true;
		goM->AddGO(lightning);
	};

private:
	//VARIABLES
	Lightning* lightning;
	GameObjectManager* goM;
	float theta;
};


#endif
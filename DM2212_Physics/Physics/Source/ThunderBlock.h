#ifndef THUNDERBLOCK_H
#define THUNDERBLOCK_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Lightning.h"


class ThunderBlock : public GameObject {
public:
	ThunderBlock(GameObjectManager* goManager)
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
		theta += 100 * dt;
		physics->SetDir(Vector3(cos(Math::DegreeToRadian(theta)), sin(Math::DegreeToRadian(theta)), 0));
		physics->SetNormal(physics->GetDir().Cross(Vector3(0,0,-1)));
		
		if (lightning != nullptr)
		{
			if (lightning->getTimer() < 0)
			{
				lightning->dead = true;
				lightning = nullptr;
			}
		}
	};

	void SpawnLightning()
	{
		lightning = new Lightning();
		lightning->physics = physics->Clone();
		lightning->Init(pos, physics->GetDir());
		lightning->active = true;
		goM->AddGO(lightning);
	};

private:
	Lightning* lightning;
	GameObjectManager* goM;
	float theta;
};


#endif
#ifndef GATEKEEPER_H
#define GATEKEEPER_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "SceneBase.h"
#include "GameObjectManager.h"
#include "MeshBuilder.h"
#include "Inventory.h"

class Gatekeeper : public GameObject {
public:

	Gatekeeper()
	{

		animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 6, 2.0f, 2.0f);
		animatedSprites->AddAnimation("idle", 18, 19);
		int turnAround[3] = { 18, 0, 6};
		animatedSprites->AddAnimation("turnAround", turnAround, 3);
		int turnBack[2] = { 6, 18 };
		animatedSprites->AddAnimation("turnBack", turnBack, 2);
		

		animatedSprites->PlayAnimation("idle", -1, 1.0f);
		mesh = animatedSprites;
		mesh->textureID = LoadTGA("Image/GatekeeperSprite.tga");
	}
	~Gatekeeper()
	{
		if (animatedSprites)
		{
			delete animatedSprites;
			animatedSprites = NULL;
		}
	}

	void Init(Inventory* inventory)
	{
		input = Input::GetInstance();

		this->inventory = inventory;
	}
	void Update(double dt)
	{
		animatedSprites->Update(dt);
	}
	bool Interact()
	{
		if (animatedSprites->GetCurrentAnimation() != "turnAround")
		{
			animatedSprites->PlayAnimation("turnAround", 0, 1.0f);
			animatedSprites->Reset();
			return false;
		}
		else if (animatedSprites->currentFrame == 6)
		{
			return true;
		}
	}

	void TurnBack()
	{
		animatedSprites->PlayAnimation("turnBack", 0, 0.5f);
		animatedSprites->Reset();
	}

	bool CheckEntry()
	{
		Item* skull = inventory->GetItem(Item::I_SKULL);
		Item* bone = inventory->GetItem(Item::I_BONE);
		if (skull != nullptr && bone != nullptr)
		{
			if (skull->GetQuantity() >= 5 && bone->GetQuantity() >= 10)
			{
				return true;
			}
		}
		return false;
	}


private:
	

	SpriteAnimation* animatedSprites;
	Input* input;
	Inventory* inventory;
};


#endif
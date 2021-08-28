#ifndef WHALE_H
#define WHALE_H

#include "GameObject.h"
#include "Physics.h"
#include "Input.h"
#include "SceneBase.h"
#include "GameObjectManager.h"
#include "MeshBuilder.h"
#include "Inventory.h"

class Whale : public GameObject {
public:

	Whale()
	{
		mesh = MeshBuilder::GenerateQuad("whale", Color(1, 1, 1), 2.0f);
		mesh->textureID = LoadTGA("Image/Tiles/OCEAN_whale.tga");
	}
	~Whale()
	{
	}

	void Init(Inventory* inventory)
	{
		input = Input::GetInstance();

		this->inventory = inventory;
	}
	void Update(double dt)
	{
	}
	bool Interact()
	{
		return true;
	}

	bool CheckEntry()
	{
		Item* blueshell = inventory->GetItem(Item::I_BLUESHELL);
		Item* redshell = inventory->GetItem(Item::I_REDSHELL);
		if (blueshell != nullptr && redshell != nullptr)
		{
			if (blueshell->GetQuantity() >= 5 && redshell->GetQuantity() >= 5)
			{
				return true;
			}
		}
		std::cout << "Whale.h : CHECKING FOR SHELLS" << std::endl;
		return false;
	}


private:
	

	SpriteAnimation* animatedSprites;
	Input* input;
	Inventory* inventory;
};


#endif
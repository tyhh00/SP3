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

	Whale() //constructor
	{
		//MESH
		mesh = MeshBuilder::GenerateQuad("whale", Color(1, 1, 1), 2.0f);
		mesh->textureID = LoadTGA("Image/Tiles/OCEAN_whale.tga");
	}
	~Whale() //destructor
	{
	}

	void Init(Inventory* inventory) //init
	{
		//INPUT
		input = Input::GetInstance();

		//INVENTORY
		this->inventory = inventory;
	}
	void Update(double dt) //update
	{
	}
	bool Interact() //interact
	{
		return true;
	}

	bool CheckEntry() //check requirement of shells
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
		return false;
	}

private:
	//INPUT
	Input* input;

	//INVENTORY
	Inventory* inventory;
};


#endif
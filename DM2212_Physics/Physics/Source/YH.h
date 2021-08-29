#ifndef YONGHONG_H
#define YONGHONG_H

#include "GameObject.h"
#include "Physics.h"
#include "Input.h"
#include "SceneBase.h"
#include "GameObjectManager.h"
#include "MeshBuilder.h"
#include "Inventory.h"

class YongHong : public GameObject {
public:

	YongHong() //constructor
	{
		//MESH
		mesh = MeshBuilder::GenerateQuad("YongHong", Color(1, 1, 1), 2.0f);
		mesh->textureID = LoadTGA("Image/Tiles/OCEAN_yh.tga");
	}
	~YongHong() //destructor
	{
	}

	void Init(Inventory* inventory)//init
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
	bool CheckEntry() //check requirements for bottle
	{
		Item* bottle = inventory->GetItem(Item::I_BOTTLE);
		if (bottle != nullptr)
		{
			if (bottle->GetQuantity() >= 1)
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
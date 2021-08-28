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

	YongHong()
	{
		mesh = MeshBuilder::GenerateQuad("YongHong", Color(1, 1, 1), 2.0f);
		mesh->textureID = LoadTGA("Image/Tiles/OCEAN_yh.tga");
	}
	~YongHong()
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
		Item* bottle = inventory->GetItem(Item::I_BOTTLE);
		if (bottle != nullptr)
		{
			if (bottle->GetQuantity() >= 1)
			{
				return true;
			}
		}
		std::cout << "YongHong.h : CHECKING FOR BOTTLE" << std::endl;
		return false;
	}


private:
	
	Input* input;
	Inventory* inventory;
};


#endif
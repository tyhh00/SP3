
#include "Tombstone.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Tombstone::Tombstone()
{
}

Tombstone::~Tombstone()
{
	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}
}

void Tombstone::Init(SceneBase* scene, Inventory* inventory)
{
	this->scene = scene;
	this->inventory = inventory;

	type = GO_TOMBSTONE;

	physics->SetEnableCollisionResponse(false);

	currentHP = 1;
	maxHP = 1; // IN SECONDS

}

void Tombstone::Update(double dt)
{ 
	mesh->material.kDiffuse.Set(1.0f, 1.0f, 1.0f);
	if (isBeingAttacked())
	{
		mesh->material.kDiffuse.Set(0.4f, 0.1f, 0.3f);
		currentHP -= dt;
	}

	if (currentHP <= 0)
	{
		dead = true;
		return;
	}

	
}

bool Tombstone::isWithinFlashlight()
{
	if (inventory->GetCurrentItem() != nullptr)
	{
		if (inventory->GetCurrentItemType() == Item::I_FLASHLIGHT)
		{
			Flashlight* flashlight = dynamic_cast<Flashlight*>(inventory->GetCurrentItem());
			if (flashlight->isWithinLight(pos))
			{
				return true;
			}
		}
	}

	return false;
}

bool Tombstone::isBeingAttacked()
{
	if (!isWithinFlashlight())
	{
		return false;
	}
	Flashlight* flashlight = dynamic_cast<Flashlight*>(inventory->GetCurrentItem());
	if (flashlight->isIntensified())
	{
		return true;
	}
	return false;
}


#include "Ghost.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Ghost::Ghost() : Enemy(GY_GHOST)
{
}

Ghost::~Ghost()
{
}

void Ghost::Init(SceneBase* scene, Inventory* inventory)
{
	this->scene = scene;
	this->inventory = inventory;

	physics->SetMovable(true);

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 2.0f, 2.0f);
	animatedSprites->AddAnimation("inactive", 9, 11);
	animatedSprites->AddAnimation("right", 6, 8);
	animatedSprites->AddAnimation("left", 3, 5);
	animatedSprites->AddAnimation("mid", 0, 2);

	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/ghost_sprite.tga");

	animatedSprites->PlayAnimation("inactive", -1, 1.0f);

}


void Ghost::Update(double dt)
{ 
	animatedSprites->Update(dt);

	if (inventory->GetCurrentItemType() == Item::I_FLASHLIGHT)
	{
		Flashlight *flashlight = dynamic_cast<Flashlight*>(inventory->GetCurrentItem());
		if (flashlight->isWithinLight(pos))
		{
			std::cout << "Is Within Light" << std::endl;
		}
	}
}

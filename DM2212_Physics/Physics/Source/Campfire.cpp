
#include "Campfire.h"
#include "Application.h"
#include "MeshBuilder.h"


Campfire::Campfire()
{
	campfireSprite = nullptr;
}

Campfire::~Campfire()
{
	delete campfireSprite;
	campfireSprite = NULL;
}

void Campfire::Init(SceneBase* _scene, Inventory* _inv, Vector3& _playerPos)
{
	campfireSprite = MeshBuilder::GenerateSpriteAnimation(1, 5, 1.0f, 1.0f);
	campfireSprite->AddAnimation("idle", 4, 5);
	campfireSprite->AddAnimation("burning", 0, 3);

	campfireSprite->textureID = LoadTGA("Image/campfire_lit.tga");
	mesh = campfireSprite;

	this->scene = _scene;
	this->inv = _inv;
	this->playerPos = _playerPos;

	campfireSprite->PlayAnimation("idle", -1, 1);
}

void Campfire::Update(double dt)
{
	campfireSprite->Update(dt);
	
	if (inv->GetCurrentItem() == nullptr)
		return;

	if (inv->GetCurrentItemType() == Item::I_FIRETORCH)
	{
		if ((playerPos - pos).Length() < 5)
		{
			campfireSprite->PlayAnimation("burning", -1, 1);
		}
	}
}

void Campfire::SetAnimation(std::string anim_name, int repeat, double anim_time)
{
	campfireSprite->PlayAnimation(anim_name, repeat, anim_time);
}
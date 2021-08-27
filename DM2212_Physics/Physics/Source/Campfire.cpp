
#include "Campfire.h"
#include "Application.h"
#include "MeshBuilder.h"


Campfire::Campfire()
{
	campfireSprite = nullptr;
	isLit = false;
	litTimer = 0;
}

bool Campfire::GetIsLit()
{
	return isLit;
}

Campfire::~Campfire()
{
	delete campfireSprite;
	campfireSprite = NULL;
}

void Campfire::Init(SceneBase* _scene, Inventory* _inv, Vector3& _playerPos)
{
	campfireSprite = MeshBuilder::GenerateSpriteAnimation(1, 5, 2.0f, 2.0f);
	campfireSprite->AddAnimation("idle", 4, 5);
	campfireSprite->AddAnimation("burning", 0, 3);

	campfireSprite->textureID = LoadTGA("Image/campfire_lit.tga");
	mesh = campfireSprite;

	this->scene = _scene;
	this->inv = _inv;
	this->playerPos = &_playerPos;

	campfireSprite->PlayAnimation("idle", -1, 1);
}

void Campfire::Update(double dt)
{
	campfireSprite->Update(dt);

	if (isLit)
	{
		if (litTimer > 30)
		{
			isLit = false;
			litTimer = 0;
		}
		campfireSprite->PlayAnimation("burning", -1, 1);
		litTimer += dt;
	}
	else
	{
		campfireSprite->PlayAnimation("idle", -1, 1);
	}
	
	if (inv->GetCurrentItem() == nullptr)
		return;

	if (inv->GetCurrentItemType() == Item::I_FIRETORCH)
	{
		if ((*playerPos - pos).Length() < 10 && !isLit)
		{
			isLit = true;
		}
	}
}

void Campfire::SetAnimation(std::string anim_name, int repeat, double anim_time)
{
	campfireSprite->PlayAnimation(anim_name, repeat, anim_time);
}
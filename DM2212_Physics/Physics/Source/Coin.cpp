#include "Coin.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "GameManager.h"

Coin::Coin(int val)
	: GameObject(GAMEOBJECT_TYPE::GO_TILE, SHAPE_TYPE::CIRCLE)
	, val(val)
{
}

void Coin::Init()
{
	geoTypeID = SceneBase::GEO_COIN;
	spriteAnimation = MeshBuilder::GenerateSpriteAnimation(1, 6, 2.0f, 2.0f);
	spriteAnimation->AddAnimation("idle", 0, 5);
	spriteAnimation->PlayAnimation("idle", -1, 1.6f);
	mesh = spriteAnimation;
	mesh->textureID = LoadTGA("Image/coin_animation.tga");

	this->physics->SetEnableCollisionResponse(false);

	yOffset = 0;
	offsetMax = 0.2;
}

Coin::~Coin()
{
	if (spriteAnimation)
	{
		delete spriteAnimation;
	}
}

void Coin::Update(double dt)
{
	spriteAnimation->Update(dt);
	this->pos.y -= yOffset;
	

}

int Coin::getCoinVal()
{
	return val;
}


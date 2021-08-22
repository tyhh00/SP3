#ifndef LOBBY_PORTAL_H
#define LOBBY_PORTAL_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

enum LobbyColor
{
	red, //graveyard
	green, //jungle
	blue, //ocean
	purple, //robot
	totalColors
};

class LobbyPortal : public GameObject {
	
private:
	SpriteAnimation* LobbyPortalSprite;
public:
	LobbyPortal::~LobbyPortal()
	{
		delete LobbyPortalSprite;
		LobbyPortalSprite = NULL;
	}

	void Init(LobbyColor color) {
		if (color == red)
		{
			LobbyPortalSprite = MeshBuilder::GenerateSpriteAnimation(5, 5, 2.0f, 2.0f);
			LobbyPortalSprite->AddAnimation("idle", 16, 17);
			LobbyPortalSprite->AddAnimation("opening", 0, 17);
			LobbyPortalSprite->AddAnimation("active", 17, 22);
			LobbyPortalSprite->textureID = LoadTGA("Image/portal_graveyard.tga");
			LobbyPortalSprite->PlayAnimation("opening", -1, 1.0f);
		}
		if (color == green)
		{
			LobbyPortalSprite = MeshBuilder::GenerateSpriteAnimation(5, 5, 2.0f, 2.0f);
			LobbyPortalSprite->AddAnimation("idle", 16, 17);
			LobbyPortalSprite->AddAnimation("opening", 0, 17);
			LobbyPortalSprite->AddAnimation("active", 17, 22);
			LobbyPortalSprite->textureID = LoadTGA("Image/portal_jungle.tga");
			LobbyPortalSprite->PlayAnimation("opening", -1, 1.0f);
		}
		if (color == blue)
		{
			LobbyPortalSprite = MeshBuilder::GenerateSpriteAnimation(5, 5, 2.0f, 2.0f);
			LobbyPortalSprite->AddAnimation("idle", 16, 17);
			LobbyPortalSprite->AddAnimation("opening", 0, 17);
			LobbyPortalSprite->AddAnimation("active", 17, 22);
			LobbyPortalSprite->textureID = LoadTGA("Image/portal_ocean.tga");
			LobbyPortalSprite->PlayAnimation("active", -1, 1.0f);
		}
		if (color == purple)
		{
			LobbyPortalSprite = MeshBuilder::GenerateSpriteAnimation(5, 5, 2.0f, 2.0f);
			LobbyPortalSprite->AddAnimation("idle", 16, 17);
			LobbyPortalSprite->AddAnimation("opening", 0, 17);
			LobbyPortalSprite->AddAnimation("active", 17, 22);
			LobbyPortalSprite->textureID = LoadTGA("Image/portal_robot.tga");
			LobbyPortalSprite->PlayAnimation("active", -1, 1.0f);
		}

		mesh = LobbyPortalSprite;
	}
	void Update(double dt) 
	{
		LobbyPortalSprite->Update(dt);
	}
};

#endif
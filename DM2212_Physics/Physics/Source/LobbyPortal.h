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

enum PortalState {

	idleState,
	openingState,
	activeState,
	closingState,
	totalState
};

class LobbyPortal : public GameObject {
	
private:
	//PORTAL STATE
	PortalState state; 

	//SPRITE ANIMATOR
	SpriteAnimation* LobbyPortalSprite;

	//VARIABLE
	double animTimer; //animation timer
	
public:

	LobbyPortal::~LobbyPortal() //deconstructor
	{
		delete LobbyPortalSprite;
		LobbyPortalSprite = NULL;
	} 
	void Init(LobbyColor color) //init
	{
		//STATE
		state = idleState;

		//CHECK PORTAL COLOR
		if (color == red)
		{
			LobbyPortalSprite = MeshBuilder::GenerateSpriteAnimation(5, 5, 2.0f, 2.0f);
			int array[17] = {16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
			LobbyPortalSprite->AddAnimation("idle", 16, 17);
			LobbyPortalSprite->AddAnimation("opening", 0, 17);
			LobbyPortalSprite->AddAnimation("closing", array, 17);
			LobbyPortalSprite->AddAnimation("active", 17, 22);
			LobbyPortalSprite->textureID = LoadTGA("Image/portal_graveyard.tga");
		}
		if (color == green)
		{
			LobbyPortalSprite = MeshBuilder::GenerateSpriteAnimation(5, 5, 2.0f, 2.0f);
			int array[17] = { 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 };
			LobbyPortalSprite->AddAnimation("idle", 16, 17);
			LobbyPortalSprite->AddAnimation("opening", 0, 17);
			LobbyPortalSprite->AddAnimation("closing", array, 17);
			LobbyPortalSprite->AddAnimation("active", 17, 22);
			LobbyPortalSprite->textureID = LoadTGA("Image/portal_jungle.tga");
		}
		if (color == blue)
		{
			LobbyPortalSprite = MeshBuilder::GenerateSpriteAnimation(5, 5, 2.0f, 2.0f);
			int array[17] = { 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 };
			LobbyPortalSprite->AddAnimation("idle", 16, 17);
			LobbyPortalSprite->AddAnimation("opening", 0, 17);
			LobbyPortalSprite->AddAnimation("closing", array, 17);
			LobbyPortalSprite->AddAnimation("active", 17, 22);
			LobbyPortalSprite->textureID = LoadTGA("Image/portal_ocean.tga");
		}
		if (color == purple)
		{
			LobbyPortalSprite = MeshBuilder::GenerateSpriteAnimation(5, 5, 2.0f, 2.0f);
			int array[17] = { 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0 };
			LobbyPortalSprite->AddAnimation("idle", 16, 17);
			LobbyPortalSprite->AddAnimation("opening", 0, 17);
			LobbyPortalSprite->AddAnimation("closing", array, 17);
			LobbyPortalSprite->AddAnimation("active", 17, 22);
			LobbyPortalSprite->textureID = LoadTGA("Image/portal_robot.tga");
		}

		//MESH
		mesh = LobbyPortalSprite;
	}
	void Update(double dt) 
	{
		//UPDATE
		LobbyPortalSprite->Update(dt);

		//SWITCH STATE
		switch (state)
		{
		case idleState:
		{
			LobbyPortalSprite->PlayAnimation("idle", -1, 1.0f);
			break;
		}
		case openingState:
		{
			if (animTimer > 0)
			{
				animTimer -= dt;
			}
			else if (animTimer <= 0)
			{
				state = activeState;
			}
			break;
		}
		case activeState:
		{
			LobbyPortalSprite->PlayAnimation("active", -1, 1.0f);
			break;
		}
		case closingState:
		{
			if (animTimer > 0)
			{
				animTimer -= dt;
			}
			else if (animTimer <= 0)
			{
				state = idleState;
			}
			break;
		}
		}
	}
	void Open() //portal open animation function
	{
		if (state == idleState)
		{
			state = openingState;
			LobbyPortalSprite->PlayAnimation("opening", 0, 1.f);
			LobbyPortalSprite->Reset();
			animTimer = 1.0;
		}
	}
	void Close() //portal close animation function
	{
		if (state == activeState)
		{
			state = closingState;
			LobbyPortalSprite->PlayAnimation("closing", 0, 1.f);
			LobbyPortalSprite->Reset();
			animTimer = 1.0;
		}
	}
	void PlayAnimation(std::string anim_name, int repeat, double anim_time) //portal play animation function
	{
		LobbyPortalSprite->PlayAnimation(anim_name, repeat, anim_time);
	}
};

#endif
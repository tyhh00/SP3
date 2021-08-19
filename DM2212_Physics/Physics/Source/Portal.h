#ifndef PORTAL_H
#define PORTAL_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Keyboard.h"
#include "SceneBase.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Ability.h"


class Portal : public GameObject {
	
private:
	SpriteAnimation* portalSprite;
public:
	Portal::~Portal()
	{
		delete portalSprite;
		portalSprite = NULL;
	}

	void Init() {
		portalSprite = MeshBuilder::GenerateSpriteAnimation(3, 8, 2.0f, 2.0f);
		portalSprite->AddAnimation("idle", 0, 7);
		portalSprite->AddAnimation("opening", 8, 15);
		portalSprite->AddAnimation("closing", 16, 22);
		//portalSprite->PlayAnimation("opening", 0, 1.0f);
		portalSprite->textureID = LoadTGA("Image/portal_sprite.tga");
		mesh = portalSprite;

		active = false;
		scale.Set(7, 7, 7);
	}
	void Update(double dt) {
		portalSprite->Update(dt);
	}
	void SetAnimation(std::string anim_name, int repeat, double anim_time) {
		portalSprite->Reset();
		portalSprite->PlayAnimation(anim_name, repeat, anim_time);
	}
};


class PortalAbilityManager {
	
public:
	PortalAbilityManager();
	~PortalAbilityManager();

	void Init();
	void Update(double dt);
	void Render(SceneBase* scene);
	void SetCamera(Camera* camera);
	void SetPlayer(Player* player);

private:

	enum ABILITY_STATE {

		DEFAULT,
		OPENING_ANIM,
		PLACING,
		PLACING_ANIM,
		TELEPORTING,
		CLOSINGSTART_ANIM,
		CLOSINGEND_ANIM,

		ASTATE_TOTAL

	};


	ABILITY_STATE state;
	Mesh* portalSprite;

	bool ghost_portal;
	bool ghost_player;
	double anim_timer;

	Portal startPortal;
	Portal endPortal;

	Camera* camera;
	Player* player;
	Keyboard* keyboard;

	void CursorToWorldPosition(double& theX, double& theY);

};

#endif
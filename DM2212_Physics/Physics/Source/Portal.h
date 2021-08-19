#ifndef PORTAL_H
#define PORTAL_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Keyboard.h"
#include "SceneBase.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"


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
		portalSprite->PlayAnimation("idle", -1, 1.0f);
		portalSprite->textureID = LoadTGA("Image/portal_sprite.tga");
		mesh = portalSprite;

		active = false;
		scale.Set(6, 6, 6);

		// CHEAT FIX - TBC; LIGHTING NOT WORKING ON SPRITE ANIMATION MESH
		mesh->material.kAmbient.Set(1, 1, 1);
	}
	void Update(double dt) {
		portalSprite->Update(dt);
	}
	void SetAnimation(std::string anim_name, int repeat, double anim_time) {
		portalSprite->PlayAnimation(anim_name, repeat, anim_time);
	}
};


class PortalAbilityManager {
	
public:
	PortalAbilityManager();

	void Init();
	void Update(Vector3 &playerPos, double dt);
	void Render(SceneBase* scene);
	void SetCamera(Camera* camera);
	void UpdateCondition(bool met);

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

	bool conditionsMet;
	bool ghost_portal;
	double anim_timer;

	Portal startPortal;
	Portal endPortal;

	Camera* camera;
	Keyboard* keyboard;

	void CursorToWorldPosition(double& theX, double& theY);

};

#endif
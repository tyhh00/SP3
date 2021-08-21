//#ifndef LOBBY_PORTAL_H
//#define LOBBY_PORTAL_H
//
//#include "GameObject.h"
//#include "SpriteAnimation.h"
//#include "Physics.h"
//#include "Input.h"
//#include "MeshBuilder.h"
//#include "LoadTGA.h"
//
//class LobbyPortal : public GameObject {
//	
//private:
//	SpriteAnimation* LobbyPortalSprite;
//public:
//	LobbyPortal::~LobbyPortal()
//	{
//		delete LobbyPortalSprite;
//		LobbyPortalSprite = NULL;
//	}
//
//	void Init() {
//		LobbyPortalSprite = MeshBuilder::GenerateSpriteAnimation(3, 8, 2.0f, 2.0f);
//		LobbyPortalSprite->AddAnimation("idle", 0, 7);
//		LobbyPortalSprite->AddAnimation("opening", 8, 15);
//		LobbyPortalSprite->AddAnimation("closing", 16, 22);
//		
//		LobbyPortalSprite->textureID = LoadTGA("Image/LobbyPortal_sprite.tga");
//		mesh = LobbyPortalSprite;
//
//		active = false;
//		scale.Set(7, 7, 7);
//	}
//	void Update(double dt) {
//		LobbyPortalSprite->Update(dt);
//	}
//	void SetAnimation(std::string anim_name, int repeat, double anim_time) {
//		//LobbyPortalSprite->Reset();
//		LobbyPortalSprite->PlayAnimation(anim_name, repeat, anim_time);
//	}
//};
//
//
//class LobbyPortalAbility : public Ability {
//	
//public:
//	LobbyPortalAbility();
//	~LobbyPortalAbility();
//
//	void Update(double dt);
//	void CustomUpdate(bool playeronGround, Vector3 playerPos);
//	void CustomUpdate(Vector3& playerPos, bool& playerInvisibility);
//	void Render();
//	ABILITY_TYPE GetAbilityType();
//
//private:
//
//	enum ABILITY_STATE {
//
//		DEFAULT,
//		OPENING_ANIM,
//		PLACING,
//		PLACING_ANIM,
//		TELEPORTING,
//		CLOSINGSTART_ANIM,
//		CLOSINGEND_ANIM,
//
//		ASTATE_TOTAL
//
//	};
//
//
//	ABILITY_STATE state;
//	Mesh* LobbyPortalSprite;
//
//	bool ghost_LobbyPortal;
//	bool ghost_player;
//	double anim_timer;
//	
//	Vector3 newPlayerPos;
//	bool conditionsMet;
//	bool playerActiveState;
//
//	LobbyPortal startLobbyPortal;
//	LobbyPortal endLobbyPortal;
//
//	void CursorToWorldPosition(double& theX, double& theY);
//
//};
//
//#endif
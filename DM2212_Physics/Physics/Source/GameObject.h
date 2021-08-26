#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Material.h"
#include "Mesh.h"
#include "Physics.h"
#include "SceneBase.h"

typedef struct Attachment Attachment;

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,

		GO_BALL,
		GO_CUBE,

		
		GO_WALL, // INELASTIC
		GO_PILLAR, // INELASTIC

		GO_FLIPPER,
		GO_SPRING, // ELASTIC + MOVING
		GO_MWALL, // INELASTIC(0.5x) + MOVING
		GO_MPILLAR, // INELASTIC(0.5x) + MOVING
		GO_ONESIDEDWALL, // INELASTIC(0.5x) + ONESIDED
		GO_MARKER, // NO COLLISION
		GO_BONUS, // ELASTIC + free coloured balls from the top
		GO_RWALL, // INELASTIC + ROTATING
		GO_RPILLAR, // INELASTIC + ROTATING
		GO_BWALL, // ELASTIC + MOVING
		GO_EPILLAR, // ELASTIC
		GO_BPILLAR, // BUMPER(2.0x) + MOVING
		GO_100, // ELASTIC + SCORE
		GO_50, // ELASTIC + SCORE
		GO_10, // ELASTIC + SCORE


		GO_PLAYER,
		GO_ENEMY,
		GO_TOMBSTONE,

		GO_TILE, //Size 2.f Object representing all Grid Tiles
		GO_TILE_DECORATIVE,

		GO_BULLET,

		GO_HIGHLIGHT,

		GO_TOTAL, //must be last
	};

	int geoTypeID;
	Mesh* mesh;
	Attachment* bottomSprite;

	GAMEOBJECT_TYPE type;

	bool dead;
	bool active;

	bool enableCollision;
	float rangeCheckMulti;

	Vector3 pos;
	Vector3 scale;

	double fireInterval;
	float maxHP, currentHP; // in use
	double timeout; // in use

	Material mat;

	Physics* physics;

	SceneBase* scene;

	//Accessible in GOManagement to enable CollidedWith(vector<GOs>)
	bool explosive; //on impact, is this explosive?
	float explosiveRadius;
	bool damagableByExplosive;

	//Temporarily disable GO (Made invisible, upon approaching timeleft 0, it will start blinking
	bool respawnableBlock;
	bool tempDisable_active;
	float tempDisable_timeleft;
	float appearLastFor; //How long this appeared shows for
	float appearDurationLeft; //How much duration left in appear state
	float reappearIn; //Respawning animation how long more till frame is showed
	float reappearCD; //After reappeared, how long more till next one shows

	//Heathbar
	bool healthBar;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE, SHAPE_TYPE shapeType = RECTANGLE);
	GameObject(GAMEOBJECT_TYPE typeValue, Mesh* mesh, int geoTypeID, SHAPE_TYPE shapeType = RECTANGLE);

	~GameObject();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render(SceneBase* scene);
	virtual GameObject* Clone();
	static void CloneValues(GameObject* ref, GameObject* into);

	virtual void CollidedWith(GameObject* go);
	void AddBottomSprite();
	//void SetScene(SceneBase* scene);

	bool IsExplosive();
	float GetExplosiveRadius();
	bool IsDamagableByExplosive();
	void SetDamagableByExplosive(bool damagableByExplosive);

	bool IsDamagable(); //Check if object is a damagable object

	void SetCurrHealth(float _currentHP);
	float GetCurrHealth();

	void SetMaxHealth(float _maxHP);
	float GetMaxHealth();

	void AddCurrHealth(float _HP);
	void MinusCurrHealth(float _HP);

	void SetTemporaryDisable(float time);
	bool IsTemporarilyDisabled();

	void SetRespawnable(bool respawnable);
	bool IsResponable();

	void SetRangeCheckMulti(float multi);
	float GetRangeCheckMulti();

	void ShowHealthBar(bool show);
};

struct Attachment : public GameObject
{
	Vector3 relativePos;
	Attachment(GAMEOBJECT_TYPE type, Vector3 relativePos, Vector3 scale) : relativePos(relativePos)
	{
		this->type = type;
		this->scale = scale;
	}
};
#endif
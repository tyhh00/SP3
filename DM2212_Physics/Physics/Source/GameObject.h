#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Material.h"
#include "Mesh.h"
#include "Physics.h"

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

		GO_TILE, //Size 2.f Object representing all Grid Tiles
		GO_TILE_DECORATIVE,

		GO_HIGHLIGHT,

		GO_TOTAL, //must be last
	};

	Mesh* mesh;

	GAMEOBJECT_TYPE type;

	bool active;
	Vector3 pos;
	Vector3 scale;
	float mass;

	double fireInterval;
	int maxHP, currentHP;
	double timeout;

	Material mat;

	Physics* physics;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE);
	GameObject(GAMEOBJECT_TYPE typeValue, Mesh* mesh) : type(typeValue), mesh(mesh)
	{}

	~GameObject();

	virtual void Init();
	virtual void Update();

};

#endif
#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"

enum ENEMY_TYPE
{
	//Robot Type Enemies
	ROBOT_ENEMY_PLASMA,
	GY_GHOST,
	GY_TUMBLEWEED,
	GY_GRIMREAPER,

	//End
	ENEMY_COUNT
};

class Enemy : public GameObject {
public:
	virtual void Init();
	virtual void Update(double dt);
	
	const ENEMY_TYPE GetEnemyType();

protected:
	Enemy(const ENEMY_TYPE type);

private:
	SpriteAnimation* animatedSprites;
	const ENEMY_TYPE type;
	//float speed;
};


#endif
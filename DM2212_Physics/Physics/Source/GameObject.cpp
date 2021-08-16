
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	momentOfInertia(1), angularVelocity(0), normal(0,1,0), dir(1, 0, 0), fireInterval(0),
	mass(1.f), vel(0, 0, 0), rotateZ(0), maxHP(0), currentHP(0), timeout(0)
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
}

void GameObject::Update()
{
}


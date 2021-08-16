
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	fireInterval(0),
	mass(1.f), maxHP(0), currentHP(0), timeout(0)
	, physics(new Physics)
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


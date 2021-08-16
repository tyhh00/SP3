
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

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, Mesh* mesh)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	fireInterval(0),
	mass(1.f),
	maxHP(0),
	currentHP(0),
	timeout(0),
	physics(new Physics),
	mesh(mesh)
{
}

GameObject::~GameObject()
{
	delete physics;
	physics = NULL;
}

void GameObject::Init()
{
}

void GameObject::Update()
{
}


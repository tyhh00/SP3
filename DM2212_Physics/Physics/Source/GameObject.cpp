
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	fireInterval(0),
	maxHP(0), currentHP(0), timeout(0),
	physics(new Physics(SceneBase::SHAPE_TYPE::RECTANGLE, pos))
{
}

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, Mesh* mesh)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	fireInterval(0),
	maxHP(0),
	currentHP(0),
	timeout(0),
	physics(new Physics(SceneBase::SHAPE_TYPE::RECTANGLE, pos)),
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



#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, SHAPE_TYPE shapeType)
	: type(typeValue),
	pos(1, 1, 1),
	scale(1, 1, 1),
	active(false),
	fireInterval(0),
	maxHP(0), currentHP(0), timeout(0), enableCollision(true),
	bottomSprite(NULL), scene(NULL)
	, explosive(false)
	, explosiveRadius(1.0f)
	, dead(false)
{
	physics = new Physics(shapeType, pos, scale);
}

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, Mesh* mesh, int geoTypeID, SHAPE_TYPE shapeType)
	: type(typeValue),
	pos(1, 1, 1),
	scale(1, 1, 1),
	active(false),
	fireInterval(0),
	geoTypeID(geoTypeID),
	maxHP(0),
	currentHP(0),
	timeout(0),
	physics(new Physics(shapeType, pos, scale)),
	mesh(mesh),
	enableCollision(true),
	bottomSprite(NULL), scene(NULL)
	, explosive(false)
	, explosiveRadius(1.0f)
	, dead(false)
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

void GameObject::Update(double dt)
{
}

void GameObject::Render(SceneBase* scene)
{
	float angle = Math::RadianToDegree(atan2(physics->GetNormal().y, physics->GetNormal().x));
	scene->modelStack.PushMatrix();
	scene->modelStack.Translate(pos.x, pos.y, pos.z);
	scene->modelStack.Rotate(angle + physics->GetRotateZ(), 0, 0, 1);
	scene->modelStack.Scale(scale.x, scale.y, scale.z);
	scene->RenderMesh(mesh, true);
	scene->modelStack.PopMatrix();
}

GameObject* GameObject::Clone()
{
	GameObject* go = new GameObject();
	go->type = this->type;
	go->active = this->active;
	go->currentHP = this->currentHP;
	go->fireInterval = this->fireInterval;
	go->geoTypeID = this->geoTypeID;
	go->mat = this->mat;
	go->maxHP = this->maxHP;
	go->mesh = this->mesh;
	go->physics = this->physics->Clone();
	go->pos = this->pos;
	go->scale = this->scale;
	go->timeout = this->timeout;
	return go;
}

void GameObject::CollidedWith(GameObject* go)
{
}

void GameObject::AddBottomSprite()
{
	bottomSprite = new Attachment(GameObject::GO_WALL,
		Vector3(0, -1 * scale.y + 0.25, 0),
		Vector3(0.5, scale.x * 0.75, 1));
	bottomSprite->physics->SetNormal(Vector3(0, 1, 0));
}

void GameObject::SetScene(SceneBase* scene)
{
	this->scene = scene;
}

bool GameObject::IsExplosive()
{
	return this->explosive;
}

float GameObject::GetExplosiveRadius()
{
	return this->explosiveRadius;
}



#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, SHAPE_TYPE shapeType)
	: type(typeValue),
	pos(1, 1, 1),
	scale(1, 1, 1),
	active(true),
	fireInterval(0),
	maxHP(100), currentHP(-1), timeout(0), enableCollision(true),
	rangeCheckMulti(1.f),
	bottomSprite(NULL), scene(NULL)
	, explosive(false)
	, explosiveRadius(1.0f)
	, dead(false)
	, damagableByExplosive(false)
	, tempDisable_active(false)
	, tempDisable_timeleft(0.0)
	, appearLastFor(0.1)
	, reappearCD(appearLastFor+0.1)
	, appearDurationLeft(appearLastFor)
	, respawnableBlock(false)
	, reappearIn(0.1)
{
	physics = new Physics(shapeType, pos, scale);
}

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, Mesh* mesh, int geoTypeID, SHAPE_TYPE shapeType)
	: type(typeValue),
	pos(1, 1, 1),
	scale(1, 1, 1),
	active(true),
	fireInterval(0),
	geoTypeID(geoTypeID),
	maxHP(100),
	currentHP(-1),
	timeout(0),
	physics(new Physics(shapeType, pos, scale)),
	mesh(mesh),
	enableCollision(true),
	rangeCheckMulti(1.f),
	bottomSprite(NULL), scene(NULL)
	, explosive(false)
	, explosiveRadius(1.0f)
	, dead(false)
	, damagableByExplosive(false)
	, tempDisable_active(false)
	, tempDisable_timeleft(0.0)
	, appearLastFor(0.4)
	, reappearCD(appearLastFor + 0.4)
	, appearDurationLeft(appearLastFor)
	, respawnableBlock(false)
	, reappearIn(0.4)
{}

bool GameObject::IsDamagable()
{
	if (currentHP < 0)
		return false;
	else
		return true;
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
	tempDisable_timeleft -= dt;

	if (tempDisable_timeleft <= 0.0 && tempDisable_active)
	{
		tempDisable_active = false;
		tempDisable_timeleft = 0.0;
		reappearIn = reappearCD;
		appearDurationLeft = appearLastFor;
		enableCollision = true;
	}
	else
	{
		appearDurationLeft -= dt;
		reappearIn -= dt;
	}
}

void GameObject::Render(SceneBase* scene)
{
	if (tempDisable_active) //Temp disable active
	{
		if (tempDisable_timeleft <= 3.0) //Whether time left is < 1
		{
			if (reappearIn <= 0.0)
			{
				reappearIn = reappearCD;
				appearDurationLeft = appearLastFor;
			}
			//If appearLastFor is less than 0,
			if (appearDurationLeft <= 0.0)
			{
				return;
			}
		}
		else {
			return;
		}
	}

	if (currentHP > 0 && currentHP < maxHP)
	{
		float healthPercen = currentHP / maxHP;

		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(pos.x, pos.y - scale.x * 1.5, pos.z);
		scene->modelStack.Scale(1.f, 0.2f, 1.f);
		scene->RenderMesh(scene->GetMesh(SceneBase::GEOMETRY_TYPE::GEO_HEALTHBAR_OUTER), false);

		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(-2.5 * (1 - healthPercen), 0.f, 0.f);
		scene->modelStack.Scale(healthPercen, 1.f, 1.f);
		scene->RenderMesh(scene->GetMesh(SceneBase::GEOMETRY_TYPE::GEO_HEALTHBAR), false);
		scene->modelStack.PopMatrix();
		scene->modelStack.PopMatrix();
	}

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
	CloneValues(this, go);
	return go;
}

void GameObject::CloneValues(GameObject* ref, GameObject* into)
{
	into->type = ref->type;
	into->active = ref->active;
	into->currentHP = ref->currentHP;
	into->fireInterval = ref->fireInterval;
	into->geoTypeID = ref->geoTypeID;
	into->mat = ref->mat;
	into->maxHP = ref->maxHP;
	into->mesh = ref->mesh;
	into->physics = ref->physics->Clone();
	into->physics->pos = Vector3(ref->physics->pos);
	into->pos = Vector3(ref->pos);
	into->scale = Vector3(ref->scale);
	into->timeout = ref->timeout;
	into->explosive = ref->explosive;
	into->explosiveRadius = ref->explosiveRadius;
	into->dead = ref->dead;
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

//void GameObject::SetScene(SceneBase* scene)
//{
//	this->scene = scene;
//}

bool GameObject::IsExplosive()
{
	return this->explosive;
}

float GameObject::GetExplosiveRadius()
{
	return this->explosiveRadius;
}

bool GameObject::IsDamagableByExplosive()
{
	return damagableByExplosive;
}

void GameObject::SetDamagableByExplosive(bool damagableByExplosive)
{
	this->damagableByExplosive = damagableByExplosive;
}

void GameObject::SetCurrHealth(float _currentHP)
{
	currentHP = _currentHP;
}

float GameObject::GetCurrHealth()
{
	return currentHP;
}

void GameObject::SetMaxHealth(float _maxHP)
{
	maxHP = _maxHP;
}

float GameObject::GetMaxHealth()
{
	return maxHP;
}

void GameObject::AddCurrHealth(float _HP)
{
	currentHP += _HP;
}

void GameObject::MinusCurrHealth(float _HP)
{
	currentHP -= _HP;
}

void GameObject::SetTemporaryDisable(float time)
{
	if (time > 3.0)
	{
		this->tempDisable_timeleft = time;
		this->tempDisable_active = true;

		this->enableCollision = false;
		
	}
}

bool GameObject::IsTemporarilyDisabled()
{
	return this->tempDisable_active;
}

void GameObject::SetRespawnable(bool respawnable)
{
	this->respawnableBlock = respawnable;
}

bool GameObject::IsResponable()
{
	return this->respawnableBlock;
}

void GameObject::SetRangeCheckMulti(float multi)
{
	this->rangeCheckMulti = multi;
}

float GameObject::GetRangeCheckMulti()
{
	return this->rangeCheckMulti;
}

void GameObject::ShowHealthBar(bool show)
{
	this->healthBar = show;
}


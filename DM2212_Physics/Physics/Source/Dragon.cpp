
#include "Dragon.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Dragon::Dragon() : Enemy(OCEAN_DRAGON)
{}

Dragon::~Dragon()
{
	for (int i = 0; i < dragon.size(); i++)
	{
		delete dragon.at(i);
		dragon.at(i) = nullptr;
	}
}

void Dragon::Init(SceneBase* scene,Vector3 &target, int numParts, GameObjectManager* goManager)
{
	this->scene = scene;
	this->inventory = inventory;
	playerPos = &target;
	goM = goManager;

	this->maxHP = 100;
	this->currentHP = 100;
	this->type = GO_DRAGON;

	angle = 0;
	curve = 10;
	curveTimer = 0.1f;

	dragonHead = new GameObject(GO_DRAGON);
	dragonHead->active = true;
	dragonHead->scale.Set(5, 5, 5);
	dragonHead->pos.Set(pos.x, pos.y, pos.z);
	dragonHead->mesh = MeshBuilder::GenerateQuad("dragonHead", Color(1, 1, 1), 2.f);
	dragonHead->mesh->textureID = LoadTGA("Image/Tiles/enemy_dragonHead.tga");
	dragonHead->invisible = true;
	dragonHead->maxHP = 110;
	dragonHead->currentHP = 110;
	dragon.push_back(dragonHead);

	for (int i = 0; i < numParts; i++)
	{
		GameObject* go = new GameObject(GO_DRAGON);
		go->active = true;
		go->scale.Set(5, 5, 5);
		go->pos.Set(pos.x + 10 + (10 * i), pos.y, pos.z);
		go->mesh = MeshBuilder::GenerateQuad("dragonBody", Color(1, 1, 1), 2.f);
		go->mesh->textureID = LoadTGA("Image/Tiles/enemy_dragonBody.tga");
		go->invisible = true;
		go->parent = dragonHead;
		dragon.push_back(go);
	}

	GameObject* go = new GameObject(GO_DRAGON);
	go->active = true;
	go->scale.Set(5, 5, 5);
	go->pos.Set(pos.x + 10 + (10 * numParts), pos.y, pos.z);
	go->mesh = MeshBuilder::GenerateQuad("dragonTail", Color(1, 1, 1), 2.f);
	go->mesh->textureID = LoadTGA("Image/Tiles/enemy_dragonTail.tga");
	go->invisible = true;
	go->parent = dragonHead;
	dragon.push_back(go);

	for (int i = 0; i < dragon.size(); i++)
	{
		goM->AddGO(dragon.at(i));
	}

	state = SINCURVE;

	activeRange = 30.0f;

	physics->SetMovable(true);
	physics->SetGravity(Vector3(0,0,0));
}

void Dragon::Update(double dt)
{ 
	if (curveTimer <= 0)
	{
		curveTimer = 0.02f;
		angle += 5;
		if (angle > 360)
		{
			angle -= 360;
		}
	}
	else
	{
		curveTimer -= dt;
	}

	if (dragonHead->currentHP <= 0)
	{
		for (int i = 0; i < dragon.size(); i++)
		{
			dragon.at(i)->dead = true;
		}
		this->dead = true;
		dragon.clear();
	}


	switch (state)
	{
	case INACTIVE:
		break;
	case SINCURVE:
		for (int i = 0; i < dragon.size(); i++)
		{
			float angleZ = (360.f / dragon.size()) * i - angle;
			dragon.at(0)->pos.y = pos.y + sin(Math::DegreeToRadian(angleZ)) * 5;
			dragon.at(i)->physics->SetRotateZ(angleZ);
			if (dragon.at(i)->physics->GetRotateZ() < 0)
			{
				dragon.at(i)->physics->SetRotateZ(dragon.at(i)->physics->GetRotateZ() + 360);
			}
			else if (dragon.at(i)->physics->GetRotateZ() > 360)
			{
				dragon.at(i)->physics->SetRotateZ(dragon.at(i)->physics->GetRotateZ() - 360);
			}
			if (dragon.at(i)->physics->GetRotateZ() > 180) //+ angleZ + angle)
			{
				dragon.at(i)->physics->SetRotateZ(360 - dragon.at(i)->physics->GetRotateZ()); //+ angleZ + angle);
			}
			dragon.at(i)->physics->SetNormal(Vector3(cos(Math::DegreeToRadian(dragon.at(i)->physics->GetRotateZ())), 
				sin(Math::DegreeToRadian(dragon.at(i)->physics->GetRotateZ())), 0));
			
		//5	dragon.at(i)->physics->SetVelocity(Vector3(-dt, dragon.at(i)->physics->GetVelocity().y, dragon.at(i)->physics->GetVelocity().z));
		}
		break;
	}


	for (int i = 1; i < dragon.size(); i++)
	{
		float angle = convertidk(dragon.at(i - 1)->physics->GetRotateZ()) + convertidk(dragon.at(i)->physics->GetRotateZ());
		dragon.at(i)->pos = dragon.at(i - 1)->pos + Vector3(dragon.at(i - 1)->scale.x * -2 * cos(Math::DegreeToRadian( 0.5 * angle)),
			dragon.at(i - 1)->scale.x * -2 * sin(Math::DegreeToRadian(0.5 * angle)), 0);
	}

}

void Dragon::Render(SceneBase* scene)
{

	for (int i = 0; i < dragon.size(); i++)
	{
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(dragon.at(i)->pos.x, dragon.at(i)->pos.y, dragon.at(i)->pos.z);
		scene->modelStack.Rotate(dragon.at(i)->physics->GetRotateZ() - 90, 0, 0, -1);
		scene->modelStack.Scale(dragon.at(i)->scale.x, dragon.at(i)->scale.y, dragon.at(i)->scale.z);
		scene->RenderMesh(dragon.at(i)->mesh, true);
		scene->modelStack.PopMatrix();
	}

	if (dragonHead->currentHP > 0 && dragonHead->currentHP < dragonHead->maxHP)
	{
		float healthPercen = dragonHead->currentHP / dragonHead->maxHP;

		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(dragonHead->pos.x, dragonHead->pos.y - dragonHead->scale.x * 1.5, dragonHead->pos.z);
		scene->modelStack.Scale(1.f, 0.2f, 1.f);
		scene->RenderMesh(scene->GetMesh(SceneBase::GEOMETRY_TYPE::GEO_HEALTHBAR_OUTER), false);

		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(-2.5 * (1 - healthPercen), 0.f, 0.f);
		scene->modelStack.Scale(healthPercen, 1.f, 1.f);
		scene->RenderMesh(scene->GetMesh(SceneBase::GEOMETRY_TYPE::GEO_HEALTHBAR), false);
		scene->modelStack.PopMatrix();
		scene->modelStack.PopMatrix();
	}

}

float Dragon::convertidk(float pain)
{
	//std::cout << "before: " << pain << std::endl;
	pain += 90;
	if (pain > 360)
	{
		pain -= 360;
	}
	pain = 360 - pain;
	//std::cout << "after: " << pain << std::endl;
	return pain;
}

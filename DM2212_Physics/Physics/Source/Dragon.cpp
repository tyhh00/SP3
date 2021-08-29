
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

void Dragon::Init(SceneBase* scene,GameObject* target, int numParts, GameObjectManager* goManager)
{
	//scene
	this->scene = scene;
	//player
	player = target;
	//game object manager
	goM = goManager;
	
	//set HP
	this->maxHP = 100;
	this->currentHP = 100;

	//set type
	this->type = GO_DRAGON;

	//Whitelist
	this->AddToGOCollisionWhitelist(GO_DRAGON);
	this->AddToResponseWhitelist(GO_DRAGON);
	this->AddToResponseWhitelist(GO_TILE);

	//Pushing back dragon GOs into dragon GO vector list
	{
		dragonHead = new GameObject(GO_DRAGON);
		dragonHead->active = true;
		dragonHead->scale.Set(5, 5, 5);
		dragonHead->pos.Set(pos.x, pos.y, pos.z);
		dragonHead->mesh = MeshBuilder::GenerateQuad("dragonHead", Color(1, 1, 1), 2.f);
		dragonHead->mesh->textureID = LoadTGA("Image/Tiles/enemy_dragonHead.tga");
		dragonHead->invisible = true;
		dragonHead->maxHP = 110;
		dragonHead->currentHP = 110;
		dragonHead->physics->SetMovable(true);
		dragonHead->physics->SetGravity(Vector3(0, 0, 0));
		dragonHead->AddToGOCollisionWhitelist(GO_DRAGON);
		dragonHead->AddToResponseWhitelist(GO_DRAGON);
		dragonHead->AddToResponseWhitelist(GO_TILE);
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
			go->AddToGOCollisionWhitelist(GO_DRAGON);
			go->AddToResponseWhitelist(GO_DRAGON);
			go->AddToResponseWhitelist(GO_TILE);
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
		go->AddToGOCollisionWhitelist(GO_DRAGON);
		go->AddToResponseWhitelist(GO_DRAGON);
		go->AddToResponseWhitelist(GO_TILE);
		dragon.push_back(go);

		//adding to GO manager
		for (int i = 0; i < dragon.size(); i++)
		{
			goM->AddGO(dragon.at(i));
		}
	}

	//Current state
	state = SINCURVE;

	//Physics
	physics->SetMovable(true);
	physics->SetGravity(Vector3(0,0,0));

	//Variables
	angle = 0;
	curve = 10;
	curveTimer = 0.1f;
}

void Dragon::Update(double dt)
{ 
	//updates dragon curving
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

	//if dragon hp < 0, then delete dragon
	if (dragonHead->currentHP <= 0)
	{
		for (int i = 0; i < dragon.size(); i++)
		{
			dragon.at(i)->dead = true;
		}
		this->dead = true;
		dragon.clear();
	}

	//updating more pos
	for (int i = 1; i < dragon.size(); i++)
	{
		float angle = convertidk(dragon.at(i - 1)->physics->GetRotateZ()) + convertidk(dragon.at(i)->physics->GetRotateZ());
		dragon.at(i)->pos = dragon.at(i - 1)->pos + Vector3(dragon.at(i - 1)->scale.x * -2 * cos(Math::DegreeToRadian(0.5 * angle)),
			dragon.at(i - 1)->scale.x * -2 * sin(Math::DegreeToRadian(0.5 * angle)), 0);
	}

	switch (state)
	{
	case SINCURVE:

		//update enemy cooldown timer before enemy attack again
		if (timeout > 0)
		{
			timeout -= dt;
		}

		for (int i = 0; i < dragon.size(); i++)
		{
			//sin curve movement
			float angleZ = (360.f / dragon.size()) * i - angle;
			dragon.at(0)->pos.y += sin(Math::DegreeToRadian(angleZ)) * 5;
			dragon.at(i)->physics->SetRotateZ(angleZ);
			if (dragon.at(i)->physics->GetRotateZ() < 0)
			{
				dragon.at(i)->physics->SetRotateZ(dragon.at(i)->physics->GetRotateZ() + 360);
			}
			else if (dragon.at(i)->physics->GetRotateZ() > 360)
			{
				dragon.at(i)->physics->SetRotateZ(dragon.at(i)->physics->GetRotateZ() - 360);
			}
			if (dragon.at(i)->physics->GetRotateZ() > 180) 
			{
				dragon.at(i)->physics->SetRotateZ(360 - dragon.at(i)->physics->GetRotateZ()); 
			}
			dragon.at(i)->physics->SetNormal(Vector3(cos(Math::DegreeToRadian(dragon.at(i)->physics->GetRotateZ())), 
				sin(Math::DegreeToRadian(dragon.at(i)->physics->GetRotateZ())), 0));
			
			//movement to the left
			dragon.at(i)->pos.x -= 10 * dt;

			//collision with player
			if (abs(dragon.at(i)->pos.x - player->pos.x) < 8)
			{
				if (abs(dragon.at(i)->pos.y - player->pos.y) < 8)
				{
					if (timeout <= 0)
					{
						player->currentHP -= 5;
						timeout = 2;
					}
				}
			}
		}
		break;
	}

	//set class pos to dragonHead pos
	this->pos = dragonHead->pos;
	this->physics->pos = dragonHead->physics->pos;
}

void Dragon::Render(SceneBase* scene)
{
	//Render dragon
	for (int i = 0; i < dragon.size(); i++)
	{
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(dragon.at(i)->pos.x, dragon.at(i)->pos.y, dragon.at(i)->pos.z);
		scene->modelStack.Rotate(dragon.at(i)->physics->GetRotateZ() - 90, 0, 0, -1);
		scene->modelStack.Scale(dragon.at(i)->scale.x, dragon.at(i)->scale.y, dragon.at(i)->scale.z);
		scene->RenderMesh(dragon.at(i)->mesh, true);
		scene->modelStack.PopMatrix();
	}

	//Dragon HP Bar
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
	pain += 90;
	if (pain > 360)
	{
		pain -= 360;
	}
	pain = 360 - pain;
	return pain;
}
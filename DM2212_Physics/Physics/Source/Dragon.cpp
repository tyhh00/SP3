
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

void Dragon::Init(SceneBase* scene,Vector3 &target, int numParts)
{
	this->scene = scene;
	this->inventory = inventory;
	playerPos = &target;

	this->maxHP = 100;
	this->currentHP = 100;
	this->type = GO_DRAGON;

	angle = 0;
	curve = 10;
	curveTimer = 0.1f;

	GameObject* go = new GameObject;
	go->active = true;
	go->scale.Set(5, 5, 5);
	go->pos.Set(pos.x, pos.y, pos.z);
	go->mesh = MeshBuilder::GenerateQuad("dragonHead", Color(1, 1, 1), 2.f);
	go->mesh->textureID = LoadTGA("Image/Tiles/enemy_dragonHead.tga");
	dragon.push_back(go);

	for (int i = 0; i < numParts; i++)
	{
		GameObject* go = new GameObject;
		go->active = true;
		go->scale.Set(5, 5, 5);
		go->pos.Set(pos.x + 10 + (10 * i), pos.y, pos.z);
		go->mesh = MeshBuilder::GenerateQuad("dragonBody", Color(1, 1, 1), 2.f);
		go->mesh->textureID = LoadTGA("Image/Tiles/enemy_dragonBody.tga");
		dragon.push_back(go);
	}

	go = new GameObject;
	go->active = true;
	go->scale.Set(5, 5, 5);
	go->pos.Set(pos.x + 10 + (10 * numParts), pos.y, pos.z);
	go->mesh = MeshBuilder::GenerateQuad("dragonTail", Color(1, 1, 1), 2.f);
	go->mesh->textureID = LoadTGA("Image/Tiles/enemy_dragonTail.tga");
	dragon.push_back(go);

	state = SINCURVE;

	activeRange = 30.0f;

	physics->SetMovable(true);
	physics->SetGravity(Vector3(0,0,0));
	animatedSprites = MeshBuilder::GenerateSpriteAnimation(2, 3, 2.0f, 2.0f);

	animatedSprites->AddAnimation("right", 3, 5);
	animatedSprites->AddAnimation("left", 1, 2);
	

	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/Dragon_sprite.tga");

	animatedSprites->PlayAnimation("left", -1, 1.0f);

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
		//5	dragon.at(i)->physics->SetVelocity(Vector3(-dt, dragon.at(i)->physics->GetVelocity().y, dragon.at(i)->physics->GetVelocity().z));
		}
		break;
	case ATTACK:
		break;
	case DEAD:
		break;
	}

	animatedSprites->Update(dt);



	for (int i = 1; i < dragon.size(); i++)
	{
		dragon.at(i)->pos = dragon.at(i - 1)->pos + Vector3(dragon.at(i - 1)->scale.x * -2 * cos(Math::DegreeToRadian(convertidk(dragon.at(i - 1)->physics->GetRotateZ()))),
			dragon.at(i - 1)->scale.x * -2 * sin(Math::DegreeToRadian(convertidk(dragon.at(i - 1)->physics->GetRotateZ()))), 0);
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

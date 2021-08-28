
#include "Lightning.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Lightning::Lightning()
{
}

Lightning::~Lightning()
{
}

void Lightning::Init(Vector3 pos, Vector3 dir)
{

	physics->SetMovable(true);
	physics->SetGravity(Vector3(0, 0, 0));
	physics->SetEnableCollisionResponse(false);

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(3, 5, 2.0f, 2.0f);
	animatedSprites->AddAnimation("thunder", 0, 11);
	
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/thunder_spriteSheet.tga");
	animatedSprites->PlayAnimation("thunder", 0, 1.f);
	animatedSprites->Reset();

	this->scale.Set(5, 20, 1);
	this->physics->SetDir(dir);
	this->pos = pos + (dir * scale.y);
	
	//this->pos = pos;
	
	//this->physics->SetDir(dir);
	this->slowedObj = false;

	this->lightningTimer = 0.8;
}

void Lightning::Update(double dt)
{ 
	animatedSprites->Update(dt);

	if (timeout > 0)
	{
		timeout -= dt;
		if (timeout < 0)
		{
			timeout = 0;
		}
	}

	if (lightningTimer > 0)
	{
		lightningTimer -= dt;
	}
}

void Lightning::CollidedWith(GameObject* go)
{
	if (go->type == GO_PLAYER)
	{
		if (timeout <= 0)
		{
			go->currentHP -= 5;
			timeout = 1.5f;
		}
	}
}

double Lightning::getTimer()
{
	return lightningTimer;
}

void Lightning::Render(SceneBase* scene)
{
	float angle = Math::RadianToDegree(this->angle + atan2(physics->GetNormal().y, physics->GetNormal().x));
	scene->modelStack.PushMatrix();
	scene->modelStack.Translate(pos.x, pos.y, pos.z);
	scene->modelStack.Rotate(angle + physics->GetRotateZ(), 0, 0, 1);
	//scene->modelStack.Translate(0, -scale.y, 0);
	scene->modelStack.Scale(scale.x, scale.y, scale.z);
	scene->RenderMesh(mesh, false);
	scene->modelStack.PopMatrix();
}

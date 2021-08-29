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
	//PHYSICS
	physics->SetMovable(true);
	physics->SetGravity(Vector3(0, 0, 0));
	physics->SetEnableCollisionResponse(false);

	//ANIMATED SPRITES
	animatedSprites = MeshBuilder::GenerateSpriteAnimation(3, 5, 2.0f, 2.0f);
	animatedSprites->AddAnimation("thunder", 0, 11);
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/thunder_spriteSheet.tga");
	animatedSprites->PlayAnimation("thunder", 0, 1.f);
	animatedSprites->Reset();

	//VARIABLES
	this->scale.Set(5, 20, 1);
	this->physics->SetDir(dir); //sets the direction lightning spawns
	this->pos = pos + (dir * this->scale.y); //sets the lightning's pos to connect with block

	this->slowedObj = false; //ensures obj is not slowed when slowtime ability is used
	this->lightningTimer = 0.8; //timer 
}

void Lightning::Update(double dt)
{ 
	animatedSprites->Update(dt); //updates animatedSprites
	if (lightningTimer > 0) //updates lightning timer
	{
		lightningTimer -= dt;
	}
}

void Lightning::CollidedWith(GameObject* go) //checks collision with objects
{
	if (go->type == GO_PLAYER)
	{
		go->currentHP -= 5;
	}
	if (go->type == GO_CRAB)
	{
		std::cout << "Collided" << std::endl;
		go->currentHP -= 100;
	}
}

double Lightning::getTimer() //gets the lightningTimer
{
	return lightningTimer;
}

void Lightning::Render(SceneBase* scene) //Renders the lightning
{
	float angle = Math::RadianToDegree(this->angle + atan2(physics->GetNormal().y, physics->GetNormal().x));
	scene->modelStack.PushMatrix();
	scene->modelStack.Translate(pos.x, pos.y, pos.z);
	scene->modelStack.Rotate(angle + physics->GetRotateZ(), 0, 0, 1);
	scene->modelStack.Scale(scale.x, scale.y, scale.z);
	scene->RenderMesh(mesh, false);
	scene->modelStack.PopMatrix();
}

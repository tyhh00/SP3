
#include "Ghost.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

void Ghost::Init()
{
	
	physics->SetMovable(true);

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 2.0f, 2.0f);
	animatedSprites->AddAnimation("inactive", 9, 11);
	animatedSprites->AddAnimation("right", 6, 8);
	animatedSprites->AddAnimation("left", 3, 5);
	animatedSprites->AddAnimation("mid", 0, 2);
	animatedSprites->PlayAnimation("inactive", -1, 1.0f);
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/ghost_sprite.tga");

	// CHEAT FIX - TBC; LIGHTING NOT WORKING ON SPRITE ANIMATION MESH
	mesh->material.kAmbient.Set(1, 1, 1);

}

void Ghost::IsInSight(float f_radius)
{
}

void Ghost::Update(Vector3 playerPos, double dt)
{ 
	animatedSprites->Update(dt);

}

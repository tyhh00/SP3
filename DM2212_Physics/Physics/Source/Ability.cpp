#include "Ability.h"

Ability::Ability(char buttonChar, ABILITY_TYPE type)
	: buttonChar(buttonChar)
	, type(type)
	, camera(nullptr)
	, goManager(nullptr)
{}

Ability::~Ability()
{

}

void Ability::SetCamera(Camera* camera)
{
	this->camera = camera;
}

void Ability::SetGOManager(GameObjectManager* goManager)
{
	this->goManager = goManager;
}

void Ability::SetScenePointer(SceneBase* scene)
{
	this->scene = scene;
}


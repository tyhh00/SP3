#include "Ability.h"

Ability::Ability(char buttonChar, ABILITY_TYPE type, double abilityCooldownDuration)
	: buttonChar(buttonChar)
	, type(type)
	, camera(nullptr)
	, goManager(nullptr)
	, abilityCooldownDuration(abilityCooldownDuration)
	, abilityCD_timeleft(0.0)
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

void Ability::SetAbilityCooldownDuration(double cooldown)
{
	this->abilityCooldownDuration = cooldown;
}

void Ability::ResetAbilityCooldown()
{
	this->abilityCD_timeleft = 0.0;
}

double Ability::GetCooldownLeft()
{
	return this->abilityCD_timeleft;
}

double Ability::GetCooldownDuration()
{
	return this->abilityCooldownDuration;
}

double Ability::GetCooldownTillReadyPercentage()
{
	if (this->abilityCD_timeleft < 0)
		this->abilityCD_timeleft = 0.0;
	return (this->abilityCooldownDuration-this->abilityCD_timeleft) / this->abilityCooldownDuration * 100;
}


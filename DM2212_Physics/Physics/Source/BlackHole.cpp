#include "BlackHole.h"

BlackHoleAbility::BlackHoleAbility(GameObject* player, BulletSpawner* spawner, float screen_w, float screen_h, Mesh* icon)
	: Ability('Q', ABILITY_TYPE::ABILITY_BLACKHOLE, 4.0, icon)
{
	this->player = player;
	this->spawner = spawner;
	this->screen_h = screen_h;
	this->screen_w = screen_w;
}

BlackHoleAbility::~BlackHoleAbility()
{
	//Clear mem
	delete spawner;
}

void BlackHoleAbility::Init()
{
}

void BlackHoleAbility::Update(double dt)
{
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0.0)
		abilityCD_timeleft = 0.0;

	if (Input::GetInstance()->IsKeyPressed(buttonChar) && abilityCD_timeleft <= 0.0)
	{
		heldDownFor += dt;

	}
	else if (Input::GetInstance()->IsKeyReleased(buttonChar) && abilityCD_timeleft <= 0.0)
	{
		double wX, wY;
		CursorToWorldPosition(wX, wY);
		Vector3 dir = Vector3(wX, wY, 1) - player->pos;
		dir.z = player->pos.z;

		float mag = heldDownFor * 0.5;
		Math::Clamp(mag, 0.1f, 2.2f);

		//spawner->SetBulletSpeed()
		spawner->SpawnBullet(player->pos, dir, dir.Normalized());
		abilityCD_timeleft = abilityCooldownDuration;
	}
}

void BlackHoleAbility::Render()
{
}

ABILITY_TYPE BlackHoleAbility::GetAbilityType()
{
	return ABILITY_TYPE::ABILITY_BLACKHOLE;
}

void BlackHoleAbility::CursorToWorldPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / screen_w);
	y = h - y;
	y /= (h / screen_h);
	x -= screen_w * 0.5 - camera->position.x;
	y -= screen_h * 0.5 - camera->position.y;

	theX = x;
	theY = y;
}

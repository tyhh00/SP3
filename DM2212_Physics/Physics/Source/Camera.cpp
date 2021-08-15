#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
	focusTarget = nullptr;
	newTarget = target;
	view_locked = false;
	SLIDE_SPEED = 100.0f;
	MAXD_X = 50.0f;
	NEWD_X = 45.0f;
	MAXD_Y = 30.0f;
	NEWD_Y = 25.0f;
	if (NEWD_X >= MAXD_X || NEWD_Y >= MAXD_Y)
		std::cout << "Camera new distance values must be smaller than max distance values!" << std::endl;
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(double dt)
{
	if (!view_locked)
	{
		if (focusTarget->pos.x - target.x > MAXD_X)
		{
			newTarget.x = focusTarget->pos.x + NEWD_X;
		}
		else if (target.x - focusTarget->pos.x > MAXD_X)
		{
			newTarget.x = focusTarget->pos.x - NEWD_X;
		}

		if (focusTarget->pos.y - target.y > MAXD_Y)
		{
			newTarget.y = focusTarget->pos.y + NEWD_Y;
		}
		else if (target.y - focusTarget->pos.y > MAXD_Y)
		{
			newTarget.y = focusTarget->pos.y - NEWD_Y;
		}
	}
	
	if (target != newTarget)
	{
		UpdateTarget(dt);
	}

	position.x = target.x;
	position.y = target.y;

}

void Camera::SetFocusTarget(GameObject* go, bool slide)
{
	focusTarget = go;
	newTarget = focusTarget->pos;
	if (!slide)
	{
		target = newTarget;
		position.x = focusTarget->pos.x;
		position.y = focusTarget->pos.y;
	}
	else
	{
		view_locked = true;
	}
}

void Camera::UpdateTarget(double dt)
{
	float offset = 3.0f;
	if ((newTarget - target).Length() < offset)
	{
		target = newTarget;
		view_locked = false;
		return;
	}
	
	target += (newTarget - target).Normalized() * SLIDE_SPEED * dt;
}

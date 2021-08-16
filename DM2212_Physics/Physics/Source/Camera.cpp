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
	view_locked = true;
	auto_lock = true;

	SLIDE_SPEED = 100.0f;
	MAXD_X = 50.0f;
	NEWD_X = 45.0f;
	MAXD_Y = 30.0f;
	NEWD_Y = 25.0f;
	if (NEWD_X >= MAXD_X || NEWD_Y >= MAXD_Y)
		std::cout << "Camera new distance values must be smaller than max distance values!" << std::endl;

	Creleased = false;
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(double dt)
{
	// use C to unlock view and move camera freely
	if (!Application::IsKeyPressed('C'))
	{
		Creleased = true;
	}
	else if (Creleased)
	{
		Creleased = false;
		ToggleAutoLock();
	}

	// if focus target exceeds maxd, set new target 
	if (view_locked)
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
	
	// move camera using arrow keys
	if (Application::IsKeyPressed(VK_LEFT))
	{
		newTarget.x = target.x - 1;
	}
	else if (Application::IsKeyPressed(VK_RIGHT))
	{
		newTarget.x = target.x + 1;
	}
	if (Application::IsKeyPressed(VK_UP))
	{
		newTarget.y = target.y + 1;
	}
	else if (Application::IsKeyPressed(VK_DOWN))
	{
		newTarget.y = target.y - 1;
	}

	// update target based on new/next target
	if (target != newTarget)
	{
		UpdateTarget(dt);
	}
	// update cam pos based on target
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
		view_locked = false;
	}
}

void Camera::ToggleAutoLock()
{
	auto_lock = !auto_lock;
	if (auto_lock)
	{
		std::cout << "Auto Lock has been turned on" << std::endl;
		view_locked = true;
	}
	else
	{
		std::cout << "Auto Lock has been turned off" << std::endl;
		view_locked = false;
	}
}

void Camera::UpdateTarget(double dt)
{
	float offset = 3.0f;
	if ((newTarget - target).Length() < offset)
	{
		target = newTarget;
		if (auto_lock)
		{
			view_locked = true;
		}
		return;
	}
	
	target += (newTarget - target).Normalized() * SLIDE_SPEED * dt;
}

#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera() : mode(CENTER)
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

	
	newTarget = target;
	view_locked = true;
	auto_lock = true;

	SLIDE_SPEED = 100.0f;
	defaultMAXD_X = 35.0f;
	defaultNEWD_X = 30.0f;
	defaultMAXD_Y = 20.0f;
	defaultNEWD_Y = 15.0f;
	MAXD_X = defaultMAXD_X;
	NEWD_X = defaultNEWD_X;
	MAXD_Y = defaultMAXD_Y;
	NEWD_Y = defaultNEWD_Y;
	
	if (NEWD_X >= MAXD_X || NEWD_Y >= MAXD_Y)

	screenWidth = screenHeight = worldWidth = worldHeight = 0;

	//Creleased = false;
	input = Input::GetInstance();

}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(Vector3 focusTarget, double dt)
{

	// use C to unlock view and move camera freely (for editing level/just viewing level purposes etc)
	// no need anym with second update func
	/*if (input->IsKeyPressed('C'))
	{		
		ToggleAutoLock();
	}*/
	
	// example of switching mode;; to be removed in final product zz
	if (input->IsKeyPressed('M'))
	{
		SetMode(CENTER);
	}
	if (input->IsKeyPressed('N'))
	{
		SetMode(EDGE);
	}

	
	

	// update target based on new/next target
	// if focus target exceeds maxd, set new target 
	if (view_locked) // if view is locked to follow focustarget
	{
		if (focusTarget.x - target.x > MAXD_X) // move camera to RIGHT
		{
			newTarget.x = focusTarget.x + NEWD_X;
		}
		else if (target.x - focusTarget.x > MAXD_X) // move camera to LEFT
		{
			newTarget.x = focusTarget.x - NEWD_X;
		}

		if (focusTarget.y - target.y > MAXD_Y) // move camera UP
		{
			newTarget.y = focusTarget.y + NEWD_Y;
		}
		else if (target.y - focusTarget.y > MAXD_Y) // move camera DOWN
		{
			newTarget.y = focusTarget.y - NEWD_Y;
		}
	}


	
	if (target != newTarget)
	{

		UpdateTarget(dt);
	}
	Constraint();

	// update cam pos based on target
	position.x = target.x;
	position.y = target.y;

}

void Camera::Update(double dt)
{
	double mousePosX, mousePosY;
	CursorToScreenPosition(mousePosX, mousePosY);


	if ((Application::IsKeyPressed(VK_LEFT)
		|| mousePosX < 0.1 * screenWidth)
		&& target.x - screenWidth * 0.5 > 0)
	{
		target.x -= 100 * dt;
	}
	else if ((Application::IsKeyPressed(VK_RIGHT)
		|| mousePosX > 0.9 * screenWidth)
		&& target.x + screenWidth * 0.5 < worldWidth)
	{
		target.x += 100 * dt;
	}
	if ((Application::IsKeyPressed(VK_UP)
		|| mousePosY > 0.9 * screenHeight)
		&& target.y + screenHeight * 0.5 < worldHeight)
	{
		target.y += 100 * dt;
	}
	else if ((Application::IsKeyPressed(VK_DOWN)
		|| mousePosY < 0.1 * screenHeight)
		&& target.y - screenHeight * 0.5 > 0)
	{
		target.y -= 100 * dt;
	}

	// update cam pos based on target
	position.x = target.x;
	position.y = target.y;

}

void Camera::SetFocusTarget(Vector3 focustarget, bool slide)
{
	newTarget = focustarget;
	if (!slide)
	{
		target = newTarget;
		position.x = focustarget.x;
		position.y = focustarget.y;
	}
	else
	{
		view_locked = false;
	}
}

void Camera::SetLimits(float screenx, float screeny, float worldx, float worldy)
{
	screenWidth = screenx;
	screenHeight = screeny;
	worldWidth = worldx;
	worldHeight = worldy;
}

void Camera::SetSlidingSpeed(float spd)
{
	SLIDE_SPEED = spd;
}

void Camera::SetMode(MODE mode)
{
	this->mode = mode;
	switch (mode)
	{
	case CENTER:
		MAXD_X = 0;
		NEWD_X = 0;
		MAXD_Y = 0;
		NEWD_Y = 0;
		break;
	case EDGE:
		MAXD_X = defaultMAXD_X;
		NEWD_X = defaultNEWD_X;
		MAXD_Y = defaultMAXD_Y;
		NEWD_Y = defaultNEWD_Y;
		break;
	default:
		break;
	}
}

void Camera::ToggleAutoLock()
{
	auto_lock = !auto_lock;
	if (auto_lock)
	{
		view_locked = true;
	}
	else
	{
		view_locked = false; 
	}
}

void Camera::CursorToScreenPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / screenWidth);
	y = h - y;
	y /= (h / screenHeight);
	//x -= screenWidth * 0.5 - position.x;
	//y -= screenHeight * 0.5 - position.y;

	theX = x;
	theY = y;
}

void Camera::ToggleAutoLock(bool on)
{
	auto_lock = on;
	if (auto_lock)
	{
		view_locked = true;
	}
	else
	{
		view_locked = false;
	}
}

void Camera::Constraint()
{
	if (target.x + screenWidth * 0.5 > worldWidth)
	{
		target.x = worldWidth - screenWidth * 0.5;
	}
	else if (target.x - screenWidth * 0.5 < 0)
	{
		target.x = screenWidth * 0.5;
	}

	if (target.y + screenHeight * 0.5 > worldHeight)
	{
		target.y = worldHeight - screenHeight * 0.5;
	}
	else if (target.y - screenHeight * 0.5 < 0)
	{
		target.y = screenHeight * 0.5;
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
	
	//target += (newTarget - target).Normalized() * SLIDE_SPEED * dt;
	target.x = Math::Lerp(target.x, newTarget.x, 0.02);
	target.y = Math::Lerp(target.y, newTarget.y, 0.02);
}

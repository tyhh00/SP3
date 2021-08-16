#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include "Vector3.h"

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	float screenWidth, screenHeight, worldWidth, worldHeight;

	float MAXD_X, MAXD_Y, NEWD_X, NEWD_Y; // offset distance before shifting and offset distance amount after shifting
	float SLIDE_SPEED; // speed of sliding of camera

	GameObject* focusTarget; // GO to focus on
	Vector3 newTarget; // target pos to slide to
	bool view_locked; // whether view is set to focus on target
	bool auto_lock; // whether view will return to target once last transition is complete

	bool Creleased; 

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);

	void SetFocusTarget(GameObject* go, bool slide = false);
	void SetLimits(float screenx, float screeny, float worldx, float worldy);
	void SetSlidingSpeed(float spd);

private:
	void UpdateTarget(double dt);
	void ToggleAutoLock();
};

#endif
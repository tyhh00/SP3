#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "Input.h"

class Camera
{
public:

	enum MODE {
		EDGE,
		CENTER,
		DELAYEDCENTER
	};

	Vector3 position;
	Vector3 target;
	Vector3 up;

	float screenWidth, screenHeight, worldWidth, worldHeight;

	float MAXD_X, MAXD_Y, NEWD_X, NEWD_Y; // offset distance before shifting and offset distance amount after shifting
	float defaultMAXD_X, defaultMAXD_Y, defaultNEWD_X, defaultNEWD_Y; // offset distance before shifting and offset distance amount after shifting
	float SLIDE_SPEED; // speed of sliding of camera

	MODE mode;

	Vector3 newTarget; // target pos to slide to
	bool view_locked; // whether view is set to focus on target
	bool auto_lock; // whether view will return to target once last transition is complete

	double delay_timer;


	Input* input;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(Vector3 focusTarget, double dt); // Update based on a target to focus on
	void Update(double dt); // Update by input; editor mode things

	void SetFocusTarget(Vector3 focustarget, bool slide = false);
	void SetLimits(float screenx, float screeny, float worldx, float worldy);
	void SetSlidingSpeed(float spd);
	void SetMode(MODE mode);
	void ToggleAutoLock(bool on);

private:
	void Constraint();
	void UpdateTarget(double dt);
	void ToggleAutoLock();
	void CursorToScreenPosition(double& theX, double& theY);
};

#endif
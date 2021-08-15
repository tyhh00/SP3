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

	float MAXD_X, MAXD_Y, NEWD_X, NEWD_Y;
	float SLIDE_SPEED;

	GameObject* focusTarget;
	Vector3 newTarget;
	bool view_locked;


	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);

	void SetFocusTarget(GameObject* go, bool slide = false);

private:
	void UpdateTarget(double dt);
};

#endif
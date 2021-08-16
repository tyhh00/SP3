#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector3.h"

class Physics {
private:
	Vector3 vel;
	Vector3 normal;
	Vector3 dir;
	float momentOfInertia;
	float angularVelocity;
public:
	Physics();
	~Physics();
	void Update();
	void SetVelocity(Vector3 vel);
	Vector3 GetVelocity();
};
#endif
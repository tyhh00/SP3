#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector3.h"

class Physics {
private:
	Vector3 vel;
	Vector3 normal;
	Vector3 dir;
	Vector3 gravity;
	float momentOfInertia;
	float angularVelocity;
public:
	Physics();
	~Physics();
	void Update(double dt);

	void SetVelocity(Vector3 _vel);
	Vector3 GetVelocity();
	
	void SetNormal(Vector3 _normal);
	Vector3 GetNormal();

	void SetDir(Vector3 _dir);
	Vector3 GetDir();

	void SetGravity(Vector3 _gravity);
	Vector3 GetGravity();
};
#endif
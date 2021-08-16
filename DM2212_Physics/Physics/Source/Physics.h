#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector3.h"
#include "SceneBase.h"

class Physics {
private:
	Vector3 pos;
	Vector3 vel;
	Vector3 normal;
	Vector3 dir;
	Vector3 gravity;
	Vector3 collisionNormal;

	float mass;
	float momentOfInertia;
	float angularVelocity;
	float rotateZ;

	SceneBase::SHAPE_TYPE shapeType;
public:
	Physics(SceneBase::SHAPE_TYPE, Vector3& pos);
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

	void SetCollisionNormal(Vector3 _collisionNormal);

	void SetMass(float _mass);
	float GetMass();

	void SetRotateZ(float _rotateZ);
	float GetRotateZ();

	void CollisionResponse(Physics* go1, Physics* go2, double dt);
	void ApplyFriction(Physics* ball, Vector3 normal, double dt);
	void ApplyInelastic(Physics* ball, Vector3 normal, double dt);
};
#endif
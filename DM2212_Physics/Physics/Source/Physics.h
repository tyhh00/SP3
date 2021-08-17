#ifndef PHYSICS_H
#define PHYSICS_H

#include "Vector3.h"
#include "Shape.h"

class Physics {
private:
	Vector3 vel;
	Vector3 normal;
	Vector3 dir;
	Vector3 gravity;
	Vector3 collisionNormal;

	float mass;
	float momentOfInertia;
	float angularVelocity;
	float rotateZ;

	bool isMovable;

public:
	Vector3 pos;
	Vector3 scale;
	SHAPE_TYPE shapeType;

	Physics(SHAPE_TYPE, Vector3 pos, Vector3 scale);
	Physics(SHAPE_TYPE, Vector3 pos, Vector3 scale, Vector3 vel, Vector3 normal, Vector3 dir, Vector3 gravity,
		Vector3 collisionNormal, float mass, float momentOfIntertia, float angularVelocity, float rotateZ,
		bool isMoveable);
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

	void SetMovable(bool _movable);
	bool GetMovable();

	void CollisionResponse(Physics* go1, Physics* go2, double dt);
	void ApplyFriction(Physics* ball, Vector3 normal, double dt);
	void ApplyInelastic(Physics* ball, Vector3 normal, double dt);
	void ApplyContactForce(Physics* go1, Physics* go2, bool applyForBall = false);

	Physics* Clone();
};
#endif
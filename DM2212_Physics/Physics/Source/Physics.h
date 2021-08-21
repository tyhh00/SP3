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
	float inelasticity;

	bool collision;
	bool isMovable;
	bool onGround;
	bool isBouncable;
	bool enableUpdate;

public:
	Vector3 pos;
	Vector3 scale;
	SHAPE_TYPE shapeType;

	Physics(SHAPE_TYPE, Vector3 pos, Vector3 scale);
	Physics(SHAPE_TYPE, Vector3 pos, Vector3 scale, Vector3 vel, Vector3 normal, Vector3 dir, Vector3 gravity,
		Vector3 collisionNormal, float mass, float momentOfIntertia, float angularVelocity, float rotateZ, float inelasticity,
		bool isMoveable, bool isBouncable);
	~Physics();
	void Update(double dt);

	void SetVelocity(Vector3 _vel);
	Vector3 GetVelocity();
	void AddVelocity(float scalar);
	void AddVelocity(Vector3 _vel);
	
	void SetNormal(Vector3 _normal);
	Vector3 GetNormal();

	void SetDir(Vector3 _dir);
	Vector3 GetDir();

	void SetGravity(Vector3 _gravity);
	Vector3 GetGravity();

	void SetOnGround(bool onGround);
	bool GetOnGround();

	void SetCollisionNormal(Vector3 _collisionNormal);

	void SetMass(float _mass);
	float GetMass();

	void SetRotateZ(float _rotateZ);
	float GetRotateZ();

	void SetMovable(bool _movable);
	bool GetMovable();

	void SetInelasticity(float _inelasticity);
	void SetIsBouncable(bool _isBouncable);

	void SetEnableUpdate(bool _enableUpdate);
	bool GetUpdateEnabled();

	void SetEnableCollision(bool _enableCollision);

	void CollisionResponse(Physics* go2, double dt);
	void ApplyFriction(Physics* ball, Vector3 normal, double dt);
	void ApplyInelastic(Physics* ball, Vector3 normal, double dt);
	void ApplyContactForce(Physics* go1, Physics* go2, double dt, bool applyForBall = false);

	Physics* Clone();
};
#endif
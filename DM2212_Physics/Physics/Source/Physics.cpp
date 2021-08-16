#include "Physics.h"

Physics::Physics()
	: vel(0, 0, 0)
	, normal(0, 1, 0)
	, dir(1, 0, 0)
	, momentOfInertia(1)
	, angularVelocity(0)
	, rotateZ(0.f)
	, gravity(0, -9.8f, 0)
{
}

Physics::~Physics()
{
}

void Physics::Update(double dt)
{
	vel.y += gravity.y * dt;
	std::cout << "vel: " << vel.y << std::endl;
}

void Physics::SetVelocity(Vector3 _vel)
{
	vel = _vel;
}

Vector3 Physics::GetVelocity()
{
	return vel;
}

void Physics::SetNormal(Vector3 _normal)
{
	normal = _normal;
}

Vector3 Physics::GetNormal()
{
	return normal;
}

void Physics::SetDir(Vector3 _dir)
{
	dir = _dir;
}

Vector3 Physics::GetDir()
{
	return dir;
}

void Physics::SetGravity(Vector3 _gravity)
{
	gravity = _gravity;
}

Vector3 Physics::GetGravity()
{
	return gravity;
}

void Physics::SetRotateZ(float _rotateZ)
{
	rotateZ = _rotateZ;
}

float Physics::GetRotateZ()
{
	return rotateZ;
}

#include "Physics.h"

Physics::Physics()
	: vel(0, 0, 0)
	, normal(0, 1, 0)
	, dir(1, 0, 0)
{
}

Physics::~Physics()
{
}

void Physics::Update()
{

}

void Physics::SetVelocity(Vector3 _vel)
{
	vel = _vel;
}

Vector3 Physics::GetVelocity()
{
	return vel;
}

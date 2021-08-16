#include "Physics.h"

Physics::Physics(SceneBase::SHAPE_TYPE _shapeType, Vector3& _pos)
	: pos(_pos)
	, vel(0, 0, 0)
	, normal(0, 1, 0)
	, dir(1, 0, 0)
	, mass(1)
	, momentOfInertia(1)
	, angularVelocity(0)
	, rotateZ(0.f)
	, gravity(0, -9.8f, 0)
	, shapeType(_shapeType)
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

void Physics::SetCollisionNormal(Vector3 _collisionNormal)
{
	collisionNormal = _collisionNormal;
}

void Physics::SetMass(float _mass)
{
	mass = _mass;
}

float Physics::GetMass()
{
	return mass;
}

void Physics::SetRotateZ(float _rotateZ)
{
	rotateZ = _rotateZ;
}

float Physics::GetRotateZ()
{
	return rotateZ;
}

void Physics::CollisionResponse(Physics* go1, Physics* go2, double dt)
{
	switch (go2->shapeType)
	{
	case GameObject::GO_BALL:
	{
		int m1 = go1->mass;
		int m2 = go2->mass;
		Vector3 u1 = go1->vel;
		Vector3 u2 = go2->vel;

		Vector3 N = (go1->pos - go2->pos).Normalized();
		go1->vel = u1 + (2 * m2 / (m1 + m2)) * ((u2 - u1).Dot(N)) * N;
		go2->vel = u2 + (2 * m1 / (m1 + m2)) * ((u1 - u2).Dot(N)) * N;
		ApplyFriction(go1, N, dt);
		ApplyFriction(go2, N, dt);

	}
	break;
	case GameObject::GO_MWALL:
	{
		ApplyFriction(go1, go2->normal, dt);
		Vector3 Rvel = go1->vel - go2->vel;
		go1->vel = go1->vel - 2 * Rvel.Dot(go2->normal) * (go2->normal);
		ApplyInelastic(go1, go2->normal, dt);
	}
	break;
	case GameObject::GO_PILLAR:
	{
		Vector3 go2pos(go2->pos.x, go2->pos.y, 0);
		Vector3 N = (go2pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);
		go1->vel = go1->vel - 2 * go1->normal.Dot(N) * N;
		ApplyInelastic(go1, N, dt);

	}
	break;
	case GameObject::GO_MPILLAR:
	{
		Vector3 go2pos(go2->pos.x, go2->pos.y, 0);
		Vector3 N = (go2pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);
		go1->vel = go1->vel - 2 * (go1->vel - go2->vel).Dot(N) * N;
		ApplyInelastic(go1, N, dt);
	}
	break;
	default:
		break;
	}
}

void Physics::ApplyFriction(Physics* ball, Vector3 normal, double dt)
{
	// - velocity parallel to surface by amount of Nforce
	const float FRICTION_K = 0.0005f;
	float Nforce = abs(Vector3(ball->mass * ball->gravity).Dot(normal));
	Vector3 plane = normal.Cross(Vector3(0, 0, 1));
	if (ball->vel.Dot(plane) < 0)
	{
		plane *= -1;
	}
	ball->vel = ball->vel - FRICTION_K * Nforce * plane * dt;
}

void Physics::ApplyInelastic(Physics* ball, Vector3 normal, double dt)
{
	// - 50% of velocity in the direction of the normal
	Vector3 iN = normal;
	if (ball->vel.Dot(iN) < 0)
	{
		iN = -1 * iN;
	}
	Vector3 proj = ball->vel.Dot(iN) * iN;
	ball->vel = ball->vel - 0.5f * proj;
}
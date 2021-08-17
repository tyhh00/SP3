/**
 Physics Class
 By: Jordan Low
 Date: 16 Aug 2021
 */
#include "Physics.h"

Physics::Physics(SHAPE_TYPE _shapeType, Vector3 _pos, Vector3 _scale)
	: pos(_pos)
	, scale(_scale)
	, vel(0, 0, 0)
	, normal(0, 1, 0)
	, dir(1, 0, 0)
	, mass(1)
	, momentOfInertia(1)
	, angularVelocity(0)
	, rotateZ(0.f)
	, gravity(0, -9.8f, 0)
	, shapeType(_shapeType)
	, isMovable(false)
{
}

Physics::Physics(SHAPE_TYPE _shapeType, Vector3 _pos, Vector3 _scale, Vector3 vel, Vector3 normal, Vector3 dir, Vector3 gravity,
	Vector3 collisionNormal, float mass, float momentOfIntertia, float angularVelocity, float rotateZ,
	bool isMoveable)
	: pos(_pos)
	, scale(_scale)
	, vel(vel)
	, normal(normal)
	, dir(dir)
	, mass(mass)
	, momentOfInertia(momentOfInertia)
	, angularVelocity(angularVelocity)
	, rotateZ(rotateZ)
	, gravity(gravity)
	, shapeType(_shapeType)
	, isMovable(isMoveable)
{}

Physics::~Physics()
{
}

/**
 @brief Update Physics
 @param dt A double for delta time
 */
void Physics::Update(double dt)
{
	vel.y += gravity.y * dt;
}

/**
 @brief Set Velocity
 @param _vel A vector3 which sets the vel
 */
void Physics::SetVelocity(Vector3 _vel)
{
	vel = _vel;
}

/**
 @brief Get Velocity
 */
Vector3 Physics::GetVelocity()
{
	return vel;
}

/**
 @brief Set Normal
 @param _normal A vector3 which sets the normal
 */
void Physics::SetNormal(Vector3 _normal)
{
	normal = _normal;
}

/**
 @brief Get Normal
 */
Vector3 Physics::GetNormal()
{
	return normal;
}

/**
 @brief Set Dir
 @param _dir A vector3 which sets the dir
 */
void Physics::SetDir(Vector3 _dir)
{
	dir = _dir;
}

/**
 @brief Get Dir
 */
Vector3 Physics::GetDir()
{
	return dir;
}

/**
 @brief Set Gravity
 @param _gravity A vector3 which sets the gravity
 */
void Physics::SetGravity(Vector3 _gravity)
{
	gravity = _gravity;
}

/**
 @brief Get Gravity
 */
Vector3 Physics::GetGravity()
{
	return gravity;
}

/**
 @brief Set Collision Normal
 @param _collisioNormal A vector3 which sets the collision Normal
 */
void Physics::SetCollisionNormal(Vector3 _collisionNormal)
{
	collisionNormal = _collisionNormal;
}

/**
 @brief Set Mass
 @param _mass A float which sets the mass
 */
void Physics::SetMass(float _mass)
{
	mass = _mass;
}

/**
 @brief Get Mass
 */
float Physics::GetMass()
{
	return mass;
}

/**
 @brief Set RotateZ
 @param _rotateZ A float which sets the rotateZ
 */
void Physics::SetRotateZ(float _rotateZ)
{
	rotateZ = _rotateZ;
}

/**
 @brief Get RotateZ
 */
float Physics::GetRotateZ()
{
	return rotateZ;
}

/**
 @brief Sets the object to be movable or not
 @param _movable A bool which sets the movable
 */
void Physics::SetMovable(bool _movable)
{
	isMovable = _movable;
}

bool Physics::GetMovable()
{
	return isMovable;
}

/**
 @brief Collision Response between 2 objects
 @param go1 A physics component of the first object
 @param go2 A physics component of the second object
 @param dt A double for delta time
 */
void Physics::CollisionResponse(Physics* go1, Physics* go2, double dt)
{
	//no need to check if go1 is movable since go1 will only represent moving entities
	if (go2->isMovable)
	{
		if (go1->shapeType == CIRCLE) //if go1 is moving circle
		{
			int m1 = go1->mass;
			int m2 = go2->mass;

			Vector3 u1 = go1->vel;
			Vector3 u2 = go2->vel;
			Vector3 N = (go1->pos - go2->pos).Normalized();
			switch (go2->shapeType)
			{
			case CIRCLE: //if go2 is moving circle
				go1->vel = u1 + (2 * m2 / (m1 + m2)) * ((u2 - u1).Dot(N)) * N;
				go2->vel = u2 + (2 * m1 / (m1 + m2)) * ((u1 - u2).Dot(N)) * N;
				ApplyFriction(go1, N, dt);
				ApplyFriction(go2, N, dt);
				break;
			case RECTANGLE: //if go2 is moving rectangle
				Vector3 N = go2->collisionNormal;
				Vector3 Rvel = go1->vel - go2->vel;
				go1->vel = go1->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(go1, N, dt);
				ApplyInelastic(go1, N, dt);
				ApplyContactForce(go1, go2);
				break;
			}
		}
		else //if go1 is moving rectangle
		{

			switch (go2->shapeType)
			{
			case CIRCLE: //if go2 is moving circle
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
			case RECTANGLE: //if go2 is moving rectangle
				Vector3 N = go2->collisionNormal;
				Vector3 Rvel = go1->vel - go2->vel;
				go1->vel = go1->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(go1, N, dt);
				ApplyInelastic(go1, N, dt);
				ApplyContactForce(go1, go2);
				break;
			}
		}
	}
	else
	{
		if (go1->shapeType == CIRCLE) //if go1 is moving circle
		{

			switch (go2->shapeType)
			{
			case CIRCLE: //if go2 is stationary circle
			{
				int m1 = go1->mass;
				int m2 = go2->mass;

				Vector3 u1 = go1->vel;
				Vector3 u2 = go2->vel;
				Vector3 N = (go1->pos - go2->pos).Normalized();

				Vector3 Rvel = go1->vel - go2->vel;
				go1->vel = go1->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(go1, N, dt);
				ApplyInelastic(go1, N, dt);
				ApplyContactForce(go1, go2, true);
			}
				break;
			case RECTANGLE: //if go2 is stationary rectangle
				Vector3 N = go2->collisionNormal;

				Vector3 Rvel = go1->vel - go2->vel;
				go1->vel = go1->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(go1, N, dt);
				ApplyInelastic(go1, N, dt);
				ApplyContactForce(go1, go2);
				break;
			}
		}
		else //if go1 is moving rectangle
		{
			switch (go2->shapeType)
			{
			case CIRCLE: //if go2 is stationary circle
			{
				//get collision normal and response
				Vector3 N = go2->collisionNormal;
				Vector3 Rvel = go1->vel - go2->vel;
				go1->vel = go1->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(go1, N, dt);
				ApplyInelastic(go1, N, dt);
				ApplyContactForce(go1, go2);
				break;
			}
			break;
			case RECTANGLE: //if go2 is stationary rectangle
				//get collision normal and response
				Vector3 N = go2->collisionNormal;
				Vector3 Rvel = go1->vel - go2->vel;
				go1->vel = go1->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(go1, N, dt);
				ApplyInelastic(go1, N, dt);
				ApplyContactForce(go1, go2);
				break;
			}
		}
	}
}

/**
 @brief Apply friction between 2 physics components
 @param ball A physics component of the first object
 @param normal A Vector3 for the normal of the second object
 @param dt A double for delta time
 */
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

/**
 @brief Apply inelastic collision between 2 physics components
 @param ball A physics component of the first object
 @param normal A Vector3 for the normal of the second object
 @param dt A double for delta time
 */
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

/**
 @brief Apply normal contact force between 2 physics components
 @param go1 A physics component of the first object
 @param go2 A physics component of the second object
 @param dt A double for delta time
 */
void Physics::ApplyContactForce(Physics* go1, Physics* go2, bool applyForBall)
{
	//get go2's collision normal
	Vector3 N = go2->collisionNormal;

	//get inward normal
	Vector3 w0_b1 = go2->pos - go1->pos;
	if (w0_b1.Dot(N) > 0)
		N = -N;

	//ensure that it doesnt sink into the object
	Vector3 projection = (w0_b1.Dot(N) / N.Dot(N)) * N; //find the projection of the direction vector on the normal
	if (projection.Length() < go1->scale.x + go2->scale.x)
	{
		Vector3 line = w0_b1 - projection;
		go1->pos = go2->pos - line + N * (go1->scale.x + go2->scale.x); //set the ball to the top of the wall in the direction of the normal
	}

	if (!applyForBall)
		return;

	//get go2's collision normal
	N = (go1->pos - go2->pos).Normalized();

	//get inward normal
	w0_b1 = go2->pos - go1->pos;
	if (w0_b1.Dot(N) > 0)
		N = -N;

	//ensure that it doesnt sink into the object
	projection = (w0_b1.Dot(N) / N.Dot(N)) * N; //find the projection of the direction vector on the normal
	if (projection.Length() < go1->scale.x + go2->scale.x)
	{
		Vector3 line = w0_b1 - projection;
		go1->pos = go2->pos - line + N * (go1->scale.x + go2->scale.x); //set the ball to the top of the wall in the direction of the normal
	}

}

Physics* Physics::Clone()
{
	Physics* newPhy = new Physics(this->shapeType, this->pos, this->scale, this->vel,
		this->normal, this->dir, this->gravity, this->collisionNormal, this->mass,
		this->momentOfInertia, this->angularVelocity, this->rotateZ, this->isMovable);
	return newPhy;
}
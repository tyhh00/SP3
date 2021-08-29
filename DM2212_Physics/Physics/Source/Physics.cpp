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
	, inelasticity(0.5f)
	, gravity(0, -120.f, 0)
	, shapeType(_shapeType)
	, isMovable(false)
	, isBouncable(true)
	, onGround(true)
	, enableUpdate(true)
	, collisionResponse(true)
	, enableGravity(true)
{
}

Physics::Physics(SHAPE_TYPE _shapeType, Vector3 _pos, Vector3 _scale, Vector3 vel, Vector3 normal, Vector3 dir, Vector3 gravity,
	Vector3 collisionNormal, float mass, float momentOfInertia, float angularVelocity, float rotateZ, float inelasticity,
	bool isMoveable, bool isBouncable)
	: pos(_pos)
	, scale(_scale)
	, vel(vel)
	, normal(normal)
	, dir(dir)
	, mass(mass)
	, momentOfInertia(momentOfInertia)
	, angularVelocity(angularVelocity)
	, rotateZ(rotateZ)
	, inelasticity(inelasticity)
	, gravity(gravity)
	, shapeType(_shapeType)
	, isMovable(isMoveable)
	, isBouncable(isBouncable)
	, onGround(true)
	, enableUpdate(true)
	, collisionResponse(true)
	, enableGravity(true)
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
	if (enableGravity)
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

void Physics::AddVelocity(float scalar)
{
	vel += scalar;
}

void Physics::AddVelocity(Vector3 _vel)
{
	vel += _vel;
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


void Physics::SetOnGround(bool onGround)
{
	this->onGround = onGround;
}

bool Physics::GetOnGround()
{
	return onGround;
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

void Physics::SetEnableUpdate(bool _enableUpdate)
{
	enableUpdate = _enableUpdate;
}

void Physics::SetEnableCollisionResponse(bool _enableCollision)
{
	collisionResponse = _enableCollision;
}

void Physics::SetGravityUpdate(bool _enableGravity)
{
	enableGravity = _enableGravity;
}

bool Physics::GetGravityUpdate()
{
	return enableGravity;
}

/**
 @brief Collision Response between 2 objects
 @param go2 A physics component of the second object
 @param dt A double for delta time
 */
void Physics::CollisionResponse(Physics* go2, double dt)
{
	if (!(this->collisionResponse && go2->collisionResponse))
		return;
	//no need to check if go1 is movable since go1 will only represent moving entities
	if (go2->isMovable)
	{
		if (this->shapeType == CIRCLE) //if go1 is moving circle
		{
			int m1 = this->mass;
			int m2 = go2->mass;

			Vector3 u1 = this->vel;
			Vector3 u2 = go2->vel;
			Vector3 N = (this->pos - go2->pos).Normalized();
			switch (go2->shapeType)
			{
			case CIRCLE: //if go2 is moving circle
				this->vel = u1 + (2 * m2 / (m1 + m2)) * ((u2 - u1).Dot(N)) * N;
				go2->vel = u2 + (2 * m1 / (m1 + m2)) * ((u1 - u2).Dot(N)) * N;
				ApplyFriction(this, N, dt);
				ApplyFriction(go2, N, dt);
				break;
			case RECTANGLE: //if go2 is moving rectangle
				Vector3 N = go2->collisionNormal;
				Vector3 Rvel = this->vel - go2->vel;
				this->vel = this->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(this, N, dt);
				ApplyInelastic(this, N, dt);
				ApplyContactForce(this, go2, dt);
				break;
			}
		}
		else //if go1 is moving rectangle
		{

			switch (go2->shapeType)
			{
			case CIRCLE: //if go2 is moving circle
			{
				int m1 = this->mass;
				int m2 = go2->mass;

				Vector3 u1 = this->vel;
				Vector3 u2 = go2->vel;
				Vector3 N = (this->pos - go2->pos).Normalized();

				this->vel = u1 + (2 * m2 / (m1 + m2)) * ((u2 - u1).Dot(N)) * N;
				go2->vel = u2 + (2 * m1 / (m1 + m2)) * ((u1 - u2).Dot(N)) * N;
				ApplyFriction(this, N, dt);
				ApplyFriction(go2, N, dt);
			}
			break;
			case RECTANGLE: //if go2 is moving rectangle
				Vector3 N = go2->collisionNormal;

				//check if this object is bouncable
				if (this->isBouncable)
				{
					Vector3 Rvel = this->vel - go2->vel;
					this->vel = this->vel - 2 * Rvel.Dot(N) * (N);
				}

				ApplyInelastic(this, N, dt);
				ApplyFriction(this, N, dt);
				ApplyContactForce(this, go2, dt);
				break;
			}
		}
	}
	else
	{
		if (this->shapeType == CIRCLE) //if go1 is moving circle
		{

			switch (go2->shapeType)
			{
			case CIRCLE: //if go2 is stationary circle
			{
				int m1 = this->mass;
				int m2 = go2->mass;

				Vector3 u1 = this->vel;
				Vector3 u2 = go2->vel;
				Vector3 N = (this->pos - go2->pos).Normalized();

				Vector3 Rvel = this->vel - go2->vel;
				this->vel = this->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(this, N, dt);
				ApplyInelastic(this, N, dt);
				ApplyContactForce(this, go2, dt, true);
			}
				break;
			case RECTANGLE: //if go2 is stationary rectangle
				Vector3 N = go2->collisionNormal;

				Vector3 Rvel = this->vel - go2->vel;
				this->vel = this->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(this, N, dt);
				ApplyInelastic(this, N, dt);
				ApplyContactForce(this, go2, dt);
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
				Vector3 Rvel = this->vel - go2->vel;
				this->vel = this->vel - 2 * Rvel.Dot(N) * (N);

				ApplyFriction(this, N, dt);
				ApplyInelastic(this, N, dt);
				ApplyContactForce(this, go2, dt);
				break;
			}
			break;
			case RECTANGLE: //if go2 is stationary rectangle
				Vector3 N = go2->collisionNormal;

				//check if this object is bouncable
				if (this->isBouncable)
				{
					Vector3 Rvel = this->vel - go2->vel;
					this->vel = this->vel - 2 * Rvel.Dot(N) * (N);
				}
				//this->pos -= this->vel * dt;

				ApplyInelastic(this, N, dt);
				ApplyFriction(this, N, dt);
				ApplyContactForce(this, go2, dt);
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
	if (ball->GetVelocity().x > -0.2f && ball->GetVelocity().x < 0.2f)
	{
		ball->SetVelocity(Vector3(0, ball->GetVelocity().y, ball->GetVelocity().z));
		return;
	}


	// - velocity parallel to surface by amount of Nforce
	const float FRICTION_K = 0.1f;
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
	ball->vel = ball->vel - ball->inelasticity * proj;
}

/**
 @brief Apply normal contact force between 2 physics components
 @param go1 A physics component of the first object
 @param go2 A physics component of the second object
 @param dt A double for delta time
 */
void Physics::ApplyContactForce(Physics* go1, Physics* go2, double dt, bool applyForBall)
{
	//get go2's collision normal
	Vector3 N = go2->collisionNormal;

	float displacement = go1->scale.x + go2->scale.x;

	//get inward normal
	Vector3 w0_b1 = go2->pos - go1->pos;
	if (w0_b1.Dot(N) > 0)
		N = -N;

	//ensure that it doesnt sink into the object
	Vector3 projection = (w0_b1.Dot(N) / N.Dot(N)) * N; //find the projection of the direction vector on the normal
	if (projection.Length() < displacement)
	{
		Vector3 line = w0_b1 - projection;
		go1->pos = go2->pos - line + N * (displacement); //set the ball to the top of the wall in the direction of the normal
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
		this->momentOfInertia, this->angularVelocity, this->rotateZ, this->inelasticity, this->isMovable, this->isBouncable);
	newPhy->collisionResponse = this->collisionResponse;
	return newPhy;
}

void Physics::SetInelasticity(float _inelasticity)
{
	inelasticity = _inelasticity;
	inelasticity = Math::Clamp(inelasticity, 0.01f, 0.99f);
}

void Physics::SetIsBouncable(bool _isBouncable)
{
	isBouncable = _isBouncable;
}

bool Physics::GetUpdateEnabled()
{
	return enableUpdate;
}

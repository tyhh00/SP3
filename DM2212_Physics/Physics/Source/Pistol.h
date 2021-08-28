#ifndef PISTOL_H
#define PISTOL_H
#include "Weapon.h"
#include "Input.h"
#include "GameObjectManager.h"
#include "LightBullet.h"
#include "Camera.h"

class Pistol : public Weapon {
public:
	Pistol(Camera* _cam, GameObjectManager* goManager, Vector3& _playerPos, LightBullet* _lightBullet, Mesh* mesh);
	~Pistol();

	void Init();
	void Update(double dt);
	bool IsEqual(Item* item1);
	
private:
	Input* input;
	LightBullet* lightBullet;
	void CursorToWorldPosition(double& theX, double& theY);
	Camera* cam;

	Vector3* playerPos;

	float shootInterval;
};


#endif
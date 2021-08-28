#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Pistol.h"

Pistol::Pistol(Camera* _cam, GameObjectManager* _goManager, Vector3& _playerPos, LightBullet* _lightBullet, Mesh* _mesh) : Weapon(I_PISTOL, mesh, bulletSpawner)
{
	bulletSpawner = new BulletSpawner(_goManager, _lightBullet);
	isStackable = false;
	mesh = _mesh;
	cam = _cam;
	playerPos = &_playerPos;

	input = Input::GetInstance();

	shootInterval = 0;
}

Pistol::~Pistol()
{
}

void Pistol::Init()
{

}

void Pistol::Update(double dt)
{
	shootInterval += dt;
	if (input->IsMousePressed(0) && shootInterval > 0.5f)
	{
		double x, y;
		CursorToWorldPosition(x, y);

		Vector3 displacement = Vector3(x, y, 0) - *playerPos;
		bulletSpawner->SpawnBullet(*playerPos, displacement, displacement);

		shootInterval = 0;
	}
}

bool Pistol::IsEqual(Item* item1)
{
	//Flashlight* checkApple = static_cast<Flashlight*>(item1);
	return false;
}

void Pistol::CursorToWorldPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / cam->screenWidth);
	y = h - y;
	y /= (h / cam->screenHeight);
	x -= cam->screenWidth * 0.5 - cam->position.x;
	y -= cam->screenHeight * 0.5 - cam->position.y;

	theX = x;
	theY = y;
}
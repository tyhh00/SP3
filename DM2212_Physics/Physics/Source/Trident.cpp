#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Trident.h"

Trident::Trident() : Weapon(I_TRIDENT)
{
	isStackable = false;
	input = Input::GetInstance();
	mesh = MeshBuilder::GenerateQuad("trident", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/Items/Weapons/Trident.tga");
}

Trident::~Trident()
{
}

void Trident::Init(Camera* cam, GameObjectManager* goManager, Vector3& pos)
{
	camera = cam;
	GOmanager = goManager;
	tridentGO = nullptr;
	playerPos = &pos;

}

void Trident::Update(double dt)
{
	double mouseposx, mouseposy;
	CursorToWorldPosition(mouseposx, mouseposy);

	if (input->IsMousePressed(0) && tridentGO == nullptr)
	{
		mouseDownPos.Set(mouseposx, mouseposy, 0);
		camera->SetMode(Camera::CENTER);
	}
	if (input->IsMouseReleased(0) && tridentGO == nullptr)
	{
		tridentGO = new TridentGO;
		tridentGO->pos = *playerPos;
		tridentGO->scale.Set(5, 5, 5);
		Vector3 mousePos(mouseposx, mouseposy, 0);
		Vector3 vel = mouseDownPos - mousePos;
		tridentGO->physics->SetVelocity(vel);
		GOmanager->AddGO(tridentGO);
		camera->SetMode(Camera::EDGE);
	}
	if (tridentGO)
	{
		float theta = atan2f(tridentGO->physics->GetVelocity().y, tridentGO->physics->GetVelocity().x);
		theta -= Math::DegreeToRadian(45);
		Vector3 normal(cos(theta), sin(theta), 0);
		tridentGO->physics->SetNormal(normal);
		if (!checkWithinScreen())
		{
			GOmanager->RemoveGO(tridentGO);
			tridentGO = nullptr;
		}
			
	}
 }

bool Trident::IsEqual(Item* item1)
{
	Trident* checkApple = static_cast<Trident*>(item1);
	return false;
}

void Trident::CursorToWorldPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / scene->GetScreenWidth());
	y = h - y;
	y /= (h / scene->GetScreenHeight());
	x -= scene->GetScreenWidth() * 0.5 - camera->position.x;
	y -= scene->GetScreenHeight() * 0.5 - camera->position.y;

	theX = x;
	theY = y;
}

bool Trident::checkWithinScreen()
{
	if (tridentGO->pos.x > camera->position.x - scene->GetScreenWidth() * 0.5
		&& tridentGO->pos.x < camera->position.x + scene->GetScreenWidth() * 0.5
		&& tridentGO->pos.y > camera->position.y - scene->GetScreenHeight() * 0.5
		&& tridentGO->pos.y < camera->position.y + scene->GetScreenHeight() * 0.5)
	{
		return true;
	}
	return false;
}

TridentGO::TridentGO()
{
	mesh = MeshBuilder::GenerateQuad("trident", Color(1,1,1), 2.0f);
	mesh->textureID = LoadTGA("Image/Items/Weapons/Trident.tga");
	physics->SetMovable(true);
	physics->SetEnableCollisionResponse(false);
	physics->SetGravity(Vector3(0, -10.f, 0));
}

TridentGO::~TridentGO()
{
	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}
}

void TridentGO::Init()
{
}

void TridentGO::Update(double dt)
{
}

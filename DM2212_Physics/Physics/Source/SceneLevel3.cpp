#include "SceneLevel3.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "MeshBuilder.h"
#include "LoadTGA.h"

SceneLevel3::SceneLevel3()
{
}

SceneLevel3::~SceneLevel3()
{
}

void SceneLevel3::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	// Meshes
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("space background", Color(1, 1, 1), 1.0f);
	meshList[GEO_BG]->textureID = LoadTGA("Image/spacebackground3.tga");

	meshList[GEO_SHIP] = MeshBuilder::GenerateQuad("spaceship", Color(1,1,1), 2.0f);
	meshList[GEO_SHIP]->textureID = LoadTGA("Image/spaceship.tga");
	meshList[GEO_BOSS] = MeshBuilder::GenerateQuad("boss triangle", Color(1, 1, 1), 2.0f);
	meshList[GEO_BOSS]->textureID = LoadTGA("Image/mrsupertriangle.tga");
	meshList[GEO_ASTEROID] = MeshBuilder::GenerateQuad("asteroid", Color(1, 1, 1), 2.0f);
	meshList[GEO_ASTEROID]->textureID = LoadTGA("Image/asteroid.tga");
	meshList[GEO_EXPLODING_ENEMY] = MeshBuilder::GenerateQuad("exploding enemy ship", Color(0, 0, 0), 2.0f);
	meshList[GEO_EXPLODING_ENEMY]->textureID = LoadTGA("Image/exploding_ship.tga");
	meshList[GEO_STANDARD_ENEMY] = MeshBuilder::GenerateQuad("standard ship", Color(0, 0, 0), 2.0f);
	meshList[GEO_STANDARD_ENEMY]->textureID = LoadTGA("Image/standardenemy.tga");
	meshList[GEO_STATIONED_ENEMY] = MeshBuilder::GenerateQuad("stationary ship", Color(0, 0, 0), 2.0f);
	meshList[GEO_STATIONED_ENEMY]->textureID = LoadTGA("Image/stationedspaceship.tga");

	meshList[GEO_BULLET] = MeshBuilder::GenerateQuad("bullet", Color(1, 1, 0), 2.0f);
	meshList[GEO_BULLET]->textureID = LoadTGA("Image/bullet.tga");
	meshList[GEO_ENEMY_BULLET] = MeshBuilder::GenerateQuad("enemy bullet", Color(1, 1, 0), 2.0f);
	meshList[GEO_ENEMY_BULLET]->textureID = LoadTGA("Image/enemybullet.tga");
	meshList[GEO_BOSS_BULLET] = MeshBuilder::GenerateQuad("boss bullet", Color(1, 1, 0), 2.0f);
	meshList[GEO_BOSS_BULLET]->textureID = LoadTGA("Image/trianglebullet.tga");
	meshList[GEO_BOSS_MISSILE] = MeshBuilder::GenerateQuad("boss missile", Color(1, 1, 0), 2.0f);
	meshList[GEO_BOSS_MISSILE]->textureID = LoadTGA("Image/triangle.tga");
	meshList[GEO_ATTACKPOWERUP] = MeshBuilder::GenerateQuad("attack powerup", Color(1, 1, 0), 2.0f);
	meshList[GEO_ATTACKPOWERUP]->textureID = LoadTGA("Image/attackpowerup.tga");
	meshList[GEO_BUDDYPOWERUP] = MeshBuilder::GenerateQuad("buddy powerup", Color(1, 1, 0), 2.0f);
	meshList[GEO_BUDDYPOWERUP]->textureID = LoadTGA("Image/buddypowerup.tga");

	meshList[GEO_EMPTYBAR] = MeshBuilder::GenerateQuad("empty bar (for boss hp)", Color(1, 1, 1), 1.0f);
	meshList[GEO_EMPTYBAR]->textureID = LoadTGA("Image/emptybar.tga");
	meshList[GEO_BOSSHP] = MeshBuilder::GenerateQuad("boss hp", Color(1, 0, 0), 1.0f);
	meshList[GEO_HPBAR] = MeshBuilder::GenerateQuad("hp bar", Color(1, 1, 1), 1.0f);
	meshList[GEO_HPBAR]->textureID = LoadTGA("Image/hpbar.tga");
	meshList[GEO_LIVES] = MeshBuilder::GenerateQuad("lives icon", Color(1, 1, 1), 1.0f);
	meshList[GEO_LIVES]->textureID = LoadTGA("Image/lives.tga");
	meshList[GEO_TARGET] = MeshBuilder::GenerateQuad("target (boss missile)", Color(1, 1, 1), 1.0f);
	meshList[GEO_TARGET]->textureID = LoadTGA("Image/target.tga");
	meshList[GEO_HOMINGWARNING] = MeshBuilder::GenerateQuad("homing warning line (explodingship)", Color(1, 1, 1), 1.0f);
	meshList[GEO_HOMINGWARNING]->textureID = LoadTGA("Image/homingline.tga");

	// Creation of Game Objects to be used
	for (int i = 0; i < 200; i++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}

	// Setting of buttons
	startButton.Set("Start", Vector3(m_worldWidth / 2, 30, 0), 30, 10);
	menuButton.Set("Main Menu", Vector3(m_worldWidth / 2, 30, 0), 30, 10);
	resumeButton.Set("Resume", Vector3(m_worldWidth / 2, 45, 0), 30, 10);
	//nextButton.Set("Next Level", Vector3(m_worldWidth / 2, 30, 0), 30, 10);
	restartButton.Set("Restart", Vector3(m_worldWidth / 2, 45, 0), 30, 10);

	// Setting of vars
	mouseClicked = false;
	mousePressed = false;

	// Init/Setting of  game related vars
	gameState = G_START;

	level_description = "Boss Battle! Mr Super Triangle wants to take over the world! We need to stop him! Defeat Mr Super Triangle to clear the level.";

	asteroidInterval = 0;
	bossStateInterval = 5;
	bossMovementInterval = 0;

	BOSS_MOVEMENT_INTERVAL = 0;
	PLAYER_FIRERATE_INTERVAL = 0.2;
	EXPLODING_SHIP_SPEED = 0.1f;

	m_lives = 3;
	m_score = 0;

	HomingState = false;

	numBullet = 1;

	m_buddy = nullptr;

	powerupType[0] = GameObject::GO_BUDDY_POWERUP;
	powerupType[1] = GameObject::GO_ATTACK_POWERUP;
	enemyType[0] = GameObject::GO_STANDARD_ENEMY;
	enemyType[1] = GameObject::GO_STATIONED_ENEMY;
	enemyType[2] = GameObject::GO_EXPLODING_ENEMY;
	enemyType[3] = GameObject::GO_BOSS;

	// Construct m_ship, setting of player/m_ship stats etc
	m_ship = new GameObject(GameObject::GO_SHIP);
	m_ship->active = true;
	m_ship->maxHP = 5;
	m_ship->currentHP = m_ship->maxHP;
	m_ship->scale.Set(5, 5, 5);
	m_ship->pos.Set(m_worldWidth * 0.25f, m_worldHeight * 0.5f, 0);
	m_ship->vel.SetZero();
	m_ship->mass = 1;
	m_ship->momentOfInertia = m_ship->mass * m_ship->scale.x * m_ship->scale.x;
	m_ship->angularVelocity = 0;
	m_ship->fireInterval = 0;

	m_boss = new GameObject(GameObject::GO_BOSS);
	m_boss->active = true;
	m_boss->maxHP = 1000;
	m_boss->currentHP = 1000;
	m_boss->scale.Set(10, 10, 10);
	m_boss->pos.Set(m_worldWidth * 0.75f, m_worldHeight * 0.5f, 0);
	m_boss->vel.SetZero();
	m_boss->fireInterval = 0;

	bossState = BS_DEFAULT;
}

GameObject* SceneLevel3::FetchGO()
{
	// Fetch a game object from m_goList and return it
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active)
		{
			return go;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		GameObject* temp = new GameObject;
		m_goList.push_back(temp);
	}
	return m_goList.back();
}

void SceneLevel3::Update(double dt)
{
	SceneBase::Update(dt);

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	// Speed of simulation
	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}
	
	// Mouse Click
	mouseClicked = false;
	if (Application::IsMousePressed(0))
	{
		mousePressed = true;
	}
	else
	{
		if (mousePressed)
		{
			mouseClicked = true;
		}
		mousePressed = false;
	}
	

	switch (gameState)
	{
	case G_START:
		if (CheckButtonClick(startButton))
		{
			gameState = G_PLAYING;
		}
		break;
	case G_PLAYING:
		UpdateGame(dt);
		// ESC to pause
		if (Application::IsKeyPressed(VK_ESCAPE))
		{
			gameState = G_PAUSED;
		}
		break;
	case G_PAUSED:
		if (CheckButtonClick(resumeButton))
		{
			gameState = G_PLAYING;
		}
		else if (CheckButtonClick(menuButton))
		{
			RestartLevel();
			Application::SwitchScene(Application::S_MAINMENU);
		}
		break;
	case G_WON:
		if (CheckButtonClick(menuButton))
		{
			RestartLevel();
			Application::SwitchScene(Application::S_MAINMENU);
		}
		break;
	case G_LOST:
		if (CheckButtonClick(restartButton))
		{
			RestartLevel();
		}
		else if (CheckButtonClick(menuButton))
		{
			RestartLevel();
			Application::SwitchScene(Application::S_MAINMENU);
		}
		break;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}


}

void SceneLevel3::UpdateGame(double dt)
{
	// Shooting of bullets
	if (Application::IsMousePressed(0) && m_ship->fireInterval >= PLAYER_FIRERATE_INTERVAL)
	{
		m_ship->fireInterval = 0;

		// Getting mouse pos 
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		x /= (w / m_worldWidth);
		y = h - y; //flip y
		y /= (h / m_worldHeight); //convert to 133:100


		// Spawning of bullet
		for (int i = 0; i < numBullet; i++)
		{
			GameObject* go = FetchGO();
			go->active = true;
			go->type = GameObject::GO_BULLET;
			go->scale.Set(1, 1, 1);
			go->pos = m_ship->pos;
			Vector3 mousePos(x, y, 0);
			go->vel = (mousePos - m_ship->pos).Normalized() * BULLET_SPEED;
			go->pos -= (go->vel.Cross(Vector3(0, 0, 1))).Normalized() * (numBullet * 0.5 * 2);
			go->pos += (go->vel.Cross(Vector3(0, 0, 1))).Normalized() * (i * 2);
			
		}
	}
	else
	{
		m_ship->fireInterval += dt;
	}

	
	// Spawning of asteroids
	if (asteroidInterval >= 4 && GetNumGOs(GameObject::GO_ASTEROID) < 6)
	{
		asteroidInterval = 0;
		GameObject* go = FetchGO();
		go->active = true;
		go->type = GameObject::GO_ASTEROID;
		go->maxHP = Math::RandIntMinMax(1, 3);
		go->mass = go->maxHP * 10;
		go->scale.Set(go->maxHP + 2, go->maxHP + 2, 1);
		go->currentHP = go->maxHP;
		//to make sure asteroid doesnt spawn on ship;; tho idk if while loop in runtime is a good idea ack
		go->pos = GetAvailablePos();
		go->vel.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
	}
	else
	{
		asteroidInterval += dt;
	}
	
	// m_ship player updates
	UpdatePlayerShip(dt);
	// boss updates
	UpdateBossShip(dt);
	// all GO updates
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			//move gos based on vel
			go->pos += go->vel * dt * m_speed;


			switch (go->type)
			{
			case GameObject::GO_ASTEROID:
				UpdateAsteroid(go);
				break;
			case GameObject::GO_BULLET:
				UpdateBullet(go);
				break;
			case GameObject::GO_ENEMY_BULLET:
				UpdateEnemyBullet(go);
				break;
			case GameObject::GO_BOSS_BULLET:
				UpdateBossBullet(go, dt);
				break;
			case GameObject::GO_BOSS_MISSILE:
				UpdateBossMissile(go, dt);
				break;
			case GameObject::GO_EXPLODING_ENEMY:
				UpdateExplodingEnemy(go, dt);
				break;
			case GameObject::GO_STANDARD_ENEMY:
				UpdateStandardEnemy(go, dt);
				break;
			case GameObject::GO_STATIONED_ENEMY:
				UpdateStationedEnemy(go, dt);
				break;
			case GameObject::GO_BUDDY:
				UpdateBuddyShip(go, dt);
				break;
			case GameObject::GO_ATTACK_POWERUP:
				UpdateAttackPowerup(go);
				break;
			case GameObject::GO_BUDDY_POWERUP:
				UpdateBuddyPowerup(go);
				break;
			default:
				break;
			}
		}
	}

	// restart level when ship dies
	if (m_ship->currentHP <= 0)
	{
		m_lives--;
		m_ship->currentHP = m_ship->maxHP;
		//RestartLevel();
	}
	// win condition
	if (m_boss->currentHP <= 0)
	{
		gameState = G_WON;
	}
	// lose condition
	if (m_lives <= 0)
	{
		gameState = G_LOST;
	}
	// PopUps
	std::vector<PopUp*>::iterator mIterator;
	for (mIterator = messageList.begin(); mIterator != messageList.end(); ++mIterator)
	{
		if ((*mIterator)->time <= 0)
		{
			delete* mIterator;
			*mIterator = nullptr;
		}
		else
		{
			(*mIterator)->time -= dt * m_speed;
		}
	}
	messageList.erase(std::remove(messageList.begin(), messageList.end(), nullptr), messageList.end());

}

void SceneLevel3::UpdatePlayerShip(double dt)
{
	// Set force based on direction
	m_force.SetZero();
	m_torque.SetZero();
	if (Application::IsKeyPressed('W'))
	{
		m_force += m_ship->dir * 100;
		m_torque = Vector3(0, -1 * m_ship->scale.y, 0).Cross(Vector3(0, 100, 0));
	}
	if (Application::IsKeyPressed('A'))
	{
		m_force += m_ship->dir * 1;
		m_torque = 3 * Vector3(m_ship->scale.x, -1 * m_ship->scale.y, 0).Cross(Vector3(0, 5, 0));
	}
	if (Application::IsKeyPressed('S'))
	{
		m_force += -1 * m_ship->dir * 100;
		m_torque = Vector3(0, m_ship->scale.y, 0).Cross(Vector3(0, -100, 0));
	}
	if (Application::IsKeyPressed('D'))
	{
		m_force += m_ship->dir * 1;
		m_torque = 3 * Vector3(-1 * m_ship->scale.x, -1 * m_ship->scale.y, 0).Cross(Vector3(0, 5, 0));
	}
	//Exercise 8: use 2 keys to increase and decrease mass of ship
	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_ship->mass += 10 * dt;
		m_ship->momentOfInertia = m_ship->mass * m_ship->scale.x * m_ship->scale.x;
	}
	else if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_ship->mass -= 10 * dt;
		m_ship->mass = Math::Max(1.f, m_ship->mass);
		m_ship->momentOfInertia = m_ship->mass * m_ship->scale.x * m_ship->scale.x;
	}

	// Physics for ship movement
	Vector3 accel = m_force * (1 / m_ship->mass);
	m_ship->vel += accel * dt * m_speed;
	if (m_ship->vel.LengthSquared() > (MAX_SPEED * MAX_SPEED))
	{
		m_ship->vel = m_ship->vel.Normalized() * MAX_SPEED;
	}
	m_ship->pos += m_ship->vel * dt * m_speed;

	// Physics for ship turning 
	float angularAccel = m_torque.z / m_ship->momentOfInertia;
	m_ship->angularVelocity += angularAccel * dt * m_speed;
	m_ship->angularVelocity = Math::Clamp(m_ship->angularVelocity, -(float)MAX_ANGULAR_SPEED, (float)MAX_ANGULAR_SPEED);
	float dTheta = m_ship->angularVelocity * dt * m_speed;
	m_ship->dir.Set(m_ship->dir.x * cosf(dTheta) - m_ship->dir.y * sin(dTheta),
		m_ship->dir.x * sinf(dTheta) + m_ship->dir.y * cosf(dTheta));

	// Warp ship if leave screen
	m_ship->pos.x = Math::Wrap(m_ship->pos.x, 0 - m_ship->scale.x, m_worldWidth + m_ship->scale.x);
	m_ship->pos.y = Math::Wrap(m_ship->pos.y, 0 - m_ship->scale.y, m_worldHeight + m_ship->scale.y);
}

void SceneLevel3::UpdateBossShip(double dt)
{
	// Check if its time to change Boss' Attack Pattern
	if (bossStateInterval >= BOSS_STATE_INTERVAL)
	{
		bossStateInterval = 0;
		//Change State
		switch (Math::RandIntMinMax(1, 10))
		{
		case 1:
		case 2:
		case 3:
			bossState = BS_CIRCLE;
			break;
		case 4:
		case 5:
		case 6:
			bossState = BS_MISSILES;
			break;
		case 7:
		case 8:
		case 9:
		case 10:
			bossState = BS_SPAWNENEMY;
			break;
		default:
			break;
		}
	}
	else
	{
		bossStateInterval += dt * m_speed;
	}

	// Update based on state
	switch (bossState)
	{
	case BS_CIRCLE:
		if (m_boss->fireInterval >= BOSS_FIRERATE_INTERVAL)
		{
			m_boss->fireInterval = 0;
			for (int angle = 0; angle < 360; angle += 45)
			{
				GameObject* bullet = FetchGO();
				bullet->active = true;
				bullet->type = GameObject::GO_BOSS_BULLET;
				bullet->pos = m_boss->pos;
				bullet->vel.Set(cos(Math::DegreeToRadian(angle)), sin(Math::DegreeToRadian(angle)), 0);
				bullet->vel = bullet->vel.Normalized() * ENEMY_BULLET_SPEED;
				bullet->scale.Set(3.5, 3, 3);
			}
		}
		else
		{
			m_boss->fireInterval += dt * m_speed;
		}
		break;
	case BS_MISSILES:
		for (int i = -1; i < 2; i += 2)
		{
			GameObject* bullet = FetchGO();
			bullet->active = true;
			bullet->type = GameObject::GO_BOSS_MISSILE;
			bullet->pos = m_boss->pos;
			bullet->vel = (m_ship->pos - m_boss->pos).Normalized() * BOSS_MISSILE_SPEED;
			bullet->vel += (bullet->vel.Cross(Vector3(0, 0, 1))).Normalized() * i * 10;
			bullet->scale.Set(3, 3, 3);
		}
		bossState = BS_DEFAULT;
		break;
	case BS_SPAWNENEMY:
		if (GetNumGOs(enemyType, sizeof(enemyType) / sizeof(int)) < 4)
		{
			GameObject* go = FetchGO();
			switch (Math::RandIntMinMax(1, 6))
			{
			case 1:
			case 2:
			case 3: //STANDARD ENEMY SHIP
				go->active = true;
				go->pos = GetAvailablePos();
				go->type = GameObject::GO_STANDARD_ENEMY;
				go->vel.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
				go->vel = go->vel.Normalized() * STANDARD_ENEMY_SHIP_SPEED;
				go->maxHP = 3;
				go->currentHP = go->maxHP;
				go->scale.Set(6, 6, 6);
				break;
			case 4:
			case 5: //STATIONARY ENEMY SHIP
				go->active = true;
				go->pos = GetAvailablePos();
				go->type = GameObject::GO_STATIONED_ENEMY;
				go->vel.SetZero();
				go->dir.SetZero();
				go->maxHP = 5;
				go->currentHP = go->maxHP;
				go->scale.Set(6, 6, 6);
				break;
			case 6: //HOMING EXPLODING ENEMY SHIP
				if (GetNumGOs(GameObject::GO_EXPLODING_ENEMY) < 1)
				{
					go->active = true;
					Vector3 displacement;
					go->pos = GetAvailablePos();
					go->type = GameObject::GO_EXPLODING_ENEMY;
					go->vel = (m_ship->pos - go->pos).Normalized() * 1.0f;
					go->maxHP = 3;
					go->currentHP = go->maxHP;
					go->scale.Set(4, 4, 4);
					HomingState = true;
					EXPLODING_SHIP_SPEED = 0.1f;
				}
				break;
			default:
				break;
			}
		}

		bossState = BS_DEFAULT;
		break;
	default:
		break;
	}

	// Check if its time to change Boss' direction
	if (bossMovementInterval >= BOSS_MOVEMENT_INTERVAL)
	{
		bossMovementInterval = 0;
		BOSS_MOVEMENT_INTERVAL = Math::RandFloatMinMax(1, 5);
		m_boss->vel.Set(Math::RandFloatMinMax(-10, 10), Math::RandFloatMinMax(-10, 10), 0);
	}
	else
	{
		bossMovementInterval += dt * m_speed;
	}
	m_boss->pos += m_boss->vel * dt * m_speed;

	// Warp ship if leave screen
	m_boss->pos.x = Math::Wrap(m_boss->pos.x, 0 - m_boss->scale.x, m_worldWidth + m_boss->scale.x);
	m_boss->pos.y = Math::Wrap(m_boss->pos.y, 0 - m_boss->scale.y, m_worldHeight + m_boss->scale.y);
}

void SceneLevel3::UpdateBuddyShip(GameObject* go, double dt)
{
	// Warp screen
	go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x, m_worldWidth + go->scale.x);
	go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y, m_worldHeight + go->scale.y);
	// follow player
	if ((go->pos - m_ship->pos).Length() > 5)
	{
		// Calculate distance to edge of screen 
		Vector3 distoedge = go->vel.Normalized();
		Vector3 temp;
		for (temp = go->pos; temp.x < m_worldWidth && temp.x > 0 && temp.y < m_worldHeight && temp.y > 0; temp += go->vel.Normalized())
		{
			distoedge += go->vel.Normalized();
		}
		if (temp.x < 0)
		{
			temp.x = m_worldWidth;
		}
		else if (temp.x > m_worldWidth)
		{
			temp.x = 0;
		}
		else if (temp.y < 0)
		{
			temp.y = m_worldHeight;
		}
		else if (temp.y > m_worldHeight)
		{
			temp.y = 0;
		}
		float distoship = distoedge.Length() + (temp - m_ship->pos).Length();
		// Warp if closer that way
		if (distoship > Vector3(m_ship->pos - go->pos).Length())
		{
			go->vel = (m_ship->pos - go->pos).Normalized() * BUDDY_SHIP_SPEED;
		}
	}
	// fire bullet
	GameObject* target = GetNearestGO(go->pos, enemyType, sizeof(enemyType)/sizeof(int));
	if (go->fireInterval >= BUDDY_FIRERATE_INTERVAL && target != nullptr)
	{
		go->fireInterval = 0;
		GameObject* bullet = FetchGO();
		bullet->active = true;
		bullet->type = GameObject::GO_BULLET;
		bullet->scale.Set(1, 1, 1);
		bullet->pos = go->pos;
		bullet->vel = (target->pos - go->pos).Normalized() * BULLET_SPEED;
	}
	else
	{
		go->fireInterval += dt;
	}
	// dir
	if (target == nullptr)
	{
		go->dir = m_ship->pos - go->pos;
	}
	else
	{
		go->dir = target->pos - go->pos;
	}
}

void SceneLevel3::UpdateBullet(GameObject* go)
{
	// Unspawn bullets when they leave screen
	if (go->pos.x >= m_worldWidth + go->scale.x
		|| go->pos.x + go->scale.x <= 0
		|| go->pos.y >= m_worldHeight + go->scale.y
		|| go->pos.y + go->scale.y <= 0)
	{
		go->active = false;
		return;
	}

	// Collision with enemy gos
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go2 = (GameObject*)*it;
		if (go2->active && CheckCollision(go, go2))
		{
			// Updates based on collided GO
			switch (go2->type)
			{
			case GameObject::GO_ASTEROID:
				go2->currentHP--;
				m_score += 10;
				if (go2->currentHP <= 0)
				{
					go2->active = false;
					if (go2->maxHP >= 2)
					{
						//spawn new smaller asteroids
						//assuming hp =/ directly proportional to mass
						int remainingHP = go2->maxHP;
						Vector3 totalMomentum = go2->vel * go2->mass;
						for (int i = 0; remainingHP > 1; ++i)
						{
							//spawn asteroid of random mass smaller than remaining HP
							GameObject* smol = FetchGO();
							smol->active = true;
							smol->type = GameObject::GO_ASTEROID;
							smol->maxHP = Math::RandIntMinMax(1, remainingHP - 1);
							smol->scale.Set(smol->maxHP + 2, smol->maxHP + 2, 1);
							smol->mass = smol->maxHP * 10;
							smol->currentHP = smol->maxHP;
							smol->pos = go2->pos;
							remainingHP -= smol->maxHP;

							smol->vel.Set(Math::RandFloatMinMax(-3, 3), Math::RandFloatMinMax(-3, 3), 0);
							totalMomentum -= smol->vel * smol->mass;

						}
						//left 1 remaining HP - last piece
						GameObject* last = FetchGO();
						last->active = true;
						last->type = GameObject::GO_ASTEROID;
						last->maxHP = 1;
						last->mass = last->maxHP * 10;
						last->scale.Set(last->maxHP + 2, last->maxHP + 2, 1);
						last->currentHP = last->maxHP;
						last->pos = go2->pos;
						remainingHP -= last->maxHP;

						last->vel = totalMomentum * (1.0f / last->mass);

						//spawn powerup
						// Spawning of power up
						if (Math::RandIntMinMax(1, 3) == 2 && GetNumGOs(powerupType, sizeof(powerupType)/sizeof(int)) < 1)
						{
							int choice = Math::RandIntMinMax(1, 2);
							
							if (choice == 1 && m_buddy == nullptr) //buddy powerup
							{
								
								GameObject* powerup = FetchGO();
								powerup->active = true;
								powerup->type = GameObject::GO_BUDDY_POWERUP;
								powerup->scale.Set(3, 3, 3);
								powerup->pos = go2->pos;
								powerup->vel.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
								
							}
							else //2 3 4 - attack powerup
							{
								GameObject * powerup = FetchGO();
								powerup->active = true;
								powerup->type = GameObject::GO_ATTACK_POWERUP;
								powerup->scale.Set(3, 3, 3);
								powerup->pos = go2->pos;
								powerup->vel.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
							
							}
						}
					}

				}
				go->active = false;
				break;
			case GameObject::GO_STANDARD_ENEMY:
				go2->currentHP--;
				m_score += 30;
				if (go2->currentHP <= 0)
				{
					go2->active = false;
				}
				go->active = false;
				break;
			case GameObject::GO_STATIONED_ENEMY:
				go2->currentHP--;
				m_score += 50;
				if (go2->currentHP <= 0)
				{
					go2->active = false;
				}
				go->active = false;
				break;
			case GameObject::GO_EXPLODING_ENEMY:
				go2->currentHP--;
				m_score += 100;
				go->active = false;
				if (go2->currentHP <= 0)
				{
					go2->active = false;
				}
				break;
			default:
				break;
			}
			// break for loop when bullet has collided
			if (!go->active)
			{
				return;
			}

		}
	}
	// Collision with boss go
	if (CheckCollision(go, m_boss))
	{
		go->active = false;
		m_boss->currentHP--;
	}
}

void SceneLevel3::UpdateEnemyBullet(GameObject* go)
{
	// Unspawn bullets when they leave screen
	if (go->pos.x >= m_worldWidth + go->scale.x
		|| go->pos.x + go->scale.x <= 0
		|| go->pos.y >= m_worldHeight + go->scale.y
		|| go->pos.y + go->scale.y <= 0)
	{
		go->active = false;
	}
	// Collision with player ship
	if (CheckCollision(go, m_ship))
	{
		go->active = false;
		m_ship->currentHP--;
	}
	// Collision with buddy ship
	else if (m_buddy != nullptr)
	{
		if (CheckCollision(go, m_buddy))
		{
			go->active = false;
			m_buddy->active = false;
			m_buddy = nullptr;
		}
	}
}

void SceneLevel3::UpdateBossBullet(GameObject* go, double dt)
{

	// Unspawn bullets when they leave screen
	if (go->pos.x >= m_worldWidth + go->scale.x
		|| go->pos.x + go->scale.x <= 0
		|| go->pos.y >= m_worldHeight + go->scale.y
		|| go->pos.y + go->scale.y <= 0)
	{
		go->active = false;
	}
	// Collision with player ship
	if (CheckCollision(go, m_ship))
	{
		go->active = false;
		m_ship->currentHP--;
	}
	// Collision with buddy ship
	else if (m_buddy != nullptr)
	{
		if (CheckCollision(go, m_buddy))
		{
			go->active = false;
			m_buddy->active = false;
			m_buddy = nullptr;
		}
	}

	// Setting of vel/accel
	Vector3 accel = (go->vel.Cross(Vector3(0, 0, -100))).Normalized() * 20.0f;
	go->vel += accel * dt * m_speed;
}

void SceneLevel3::UpdateBossMissile(GameObject* go, double dt)
{
	// Unspawn bullets when they leave screen
	if (go->pos.x >= m_worldWidth + go->scale.x
		|| go->pos.x + go->scale.x <= 0
		|| go->pos.y >= m_worldHeight + go->scale.y
		|| go->pos.y + go->scale.y <= 0)
	{
		go->active = false;
	}
	// Collision with player ship
	if (CheckCollision(go, m_ship))
	{
		go->active = false;
		m_ship->currentHP--;
	}
	// Collision with buddy ship
	else if (m_buddy != nullptr)
	{
		if (CheckCollision(go, m_buddy))
		{
			go->active = false;
			m_buddy->active = false;
			m_buddy = nullptr;
		}
	}
	// to check if missile is heading towards edge (when player ship warps to another end)
	// to prevent situation where missiles turns around when player warps, extending the distance suddenly
	// instead, missile will simply leave the screen if its alr heading to the edge when player warps
	Vector3 distoedge = go->vel.Normalized();
	for (Vector3 temp = go->pos; temp.x < m_worldWidth && temp.x > 0 && temp.y < m_worldHeight && temp.y > 0; temp += go->vel.Normalized())
	{
		distoedge += go->vel.Normalized();
	}
	if (distoedge.Length() > (m_ship->pos - go->pos).Length())
	{
		//Setting of vel/accel
		Vector3 accel = ((m_ship->pos - go->pos) - go->vel).Normalized() * 200;
		go->vel += accel * dt * m_speed;
		go->vel = go->vel.Normalized() * BOSS_MISSILE_SPEED;
	} //else just leave screen ;;vel does not change

}

void SceneLevel3::UpdateAsteroid(GameObject* go)
{
	// Collision with player ship
	if (CheckCollision(m_ship, go))
	{
		m_ship->currentHP--;
		m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
		m_ship->vel.Set(0, 0, 0);
		go->active = false;
	}
	// Collision with buddy ship
	else if (m_buddy != nullptr)
	{
		if (CheckCollision(go, m_buddy))
		{
			go->active = false;
			m_buddy->active = false;
			m_buddy = nullptr;
		}
	}
	// Warp screen
	go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x, m_worldWidth + go->scale.x);
	go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y, m_worldHeight + go->scale.y);
}

void SceneLevel3::UpdateStandardEnemy(GameObject* go, double dt)
{
	// Warp screen
	go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x, m_worldWidth + go->scale.x);
	go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y, m_worldHeight + go->scale.y);
	// Fire Bullets
	if (go->fireInterval >= ENEMY_FIRERATE_INTERVAL)
	{
		go->fireInterval = 0;
		for (int i = -1; i < 2; i += 2)
		{
			GameObject* bullet = FetchGO();
			bullet->active = true;
			bullet->type = GameObject::GO_ENEMY_BULLET;
			bullet->scale.Set(1, 1, 1);
			bullet->pos = go->pos + ((go->vel.Cross(Vector3(0, 0, 1))).Normalized() * i * 3);
			bullet->vel = go->vel.Normalized() * ENEMY_BULLET_SPEED;

		}
	}
	else
	{
		go->fireInterval += dt;
	}
	// Collision with player ship
	if (CheckCollision(go, m_ship))
	{
		go->active = false;
		m_ship->currentHP--;
	}
	// Collision with buddy ship
	else if (m_buddy != nullptr)
	{
		if (CheckCollision(go, m_buddy))
		{
			go->active = false;
			m_buddy->active = false;
			m_buddy = nullptr;
		}
	}
}

void SceneLevel3::UpdateStationedEnemy(GameObject* go, double dt)
{
	if (CheckCollision(go, m_ship))
	{
		go->active = false;
		m_ship->currentHP--;
	}
	// Collision with buddy ship
	else if (m_buddy != nullptr)
	{
		if (CheckCollision(go, m_buddy))
		{
			go->active = false;
			m_buddy->active = false;
			m_buddy = nullptr;
		}
	}

	go->dir = m_ship->pos - go->pos;
	if (go->fireInterval >= ENEMY_FIRERATE_INTERVAL)
	{
		go->fireInterval = 0;

		GameObject* bullet = FetchGO();
		bullet->active = true;
		bullet->type = GameObject::GO_ENEMY_BULLET;
		bullet->scale.Set(1, 1, 1);
		bullet->pos = go->pos;
		bullet->vel = (m_ship->pos - go->pos).Normalized() * ENEMY_BULLET_SPEED;

	}
	else
	{
		go->fireInterval += dt;
	}

}

void SceneLevel3::UpdateExplodingEnemy(GameObject* go, double dt)
{
	// Unspawn ship when they leave screen
	if (go->pos.x >= m_worldWidth + go->scale.x
		|| go->pos.x + go->scale.x <= 0
		|| go->pos.y >= m_worldHeight + go->scale.y
		|| go->pos.y + go->scale.y <= 0)
	{
		go->active = false;
	}
	// to check if homing ship is heading towards edge (when player ship warps to another end)
	// to prevent situation where exploding ship turns around when player warps, extending the distance suddenly
	// instead, homing exploding enemy ship will simply leave the screen if its alr heading to the edge when player warps
	Vector3 distoedge = go->vel.Normalized();
	for (Vector3 temp = go->pos; temp.x < m_worldWidth && temp.x > 0 && temp.y < m_worldHeight && temp.y > 0; temp += go->vel.Normalized())
	{
		distoedge += go->vel.Normalized();
	}

	if (CheckCollision(go, m_ship)) // collision with player ship
	{
		go->active = false;
		m_ship->currentHP--;
	}
	// Collision with buddy ship
	else if (m_buddy != nullptr)
	{
		if (CheckCollision(go, m_buddy))
		{
			go->active = false;
			m_buddy->active = false;
			m_buddy = nullptr;
		}
	}
	if (distoedge.Length() < Vector3(m_ship->pos - go->pos).Length()) // player warped;; in opposite dir or is much further away as compared to distance to edge of screen
	{
		HomingState = false;
	}
	else // ship is chasing player
	{
		HomingState = true;
	}

	if (HomingState) // default chasing state
	{
		EXPLODING_SHIP_SPEED += 2 * dt;
		go->vel = (m_ship->pos - go->pos).Normalized() * EXPLODING_SHIP_SPEED;
		go->rotateZ = Math::RadianToDegree(atan2(go->vel.y, go->vel.x));
	}
}

void SceneLevel3::UpdateAttackPowerup(GameObject* go)
{
	// Warp screen
	go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x, m_worldWidth + go->scale.x);
	go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y, m_worldHeight + go->scale.y);
	// Pick Up
	if (CheckCollision(go, m_ship))
	{
		go->active = false;
		switch (Math::RandIntMinMax(2, 3))
		{
		case 2: //firerate increase
			PLAYER_FIRERATE_INTERVAL *= 0.75;
			messageList.push_back(new PopUp("Player Firerate Increased!!", Vector3(m_worldWidth * 0.5f - 7, m_worldHeight * 0.5f, 2), Color(1, 1, 1), 2));
			break;
		case 3: //extra bullet
			numBullet++;
			messageList.push_back(new PopUp("Number of bullets Increased!!", Vector3(m_worldWidth * 0.5f - 7, m_worldHeight * 0.5f, 2), Color(1, 1, 1), 2));
			break;
		}
	}
}

void SceneLevel3::UpdateBuddyPowerup(GameObject* go)
{
	// Warp screen
	go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x, m_worldWidth + go->scale.x);
	go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y, m_worldHeight + go->scale.y);
	// Check pick up
	if (CheckCollision(m_ship, go))
	{
		go->active = false;
		GameObject* go = FetchGO();
		m_buddy = go;
		go->active = true;
		go->type = GameObject::GO_BUDDY;
		go->maxHP = 1;
		go->scale.Set(2, 2, 2);
		go->currentHP = go->maxHP;
		go->pos = m_ship->pos - (m_ship->vel.Normalized() * 5);
		go->vel = m_ship->pos - go->pos;
	}
}

void SceneLevel3::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_SHIP:
		//ship
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(m_ship->dir.y, m_ship->dir.x)) + 90, 0, 0, 1); //+90 to make 0 degrees the positive x axis
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SHIP], true);
		modelStack.PopMatrix();
		//hp bar
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x - ((go->maxHP - go->currentHP) * (1.0f / go->maxHP) * 5.0f), go->pos.y + go->scale.y + 3, go->pos.z);
		modelStack.Scale(go->currentHP * (1.0f / go->maxHP) * 10, 3, 1);
		RenderMesh(meshList[GEO_HPBAR], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOSS:
		//ship
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(180, 0, 0, 1); 
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BOSS], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ASTEROID:
		
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ASTEROID], true);
		modelStack.PopMatrix();

		if (go->currentHP != go->maxHP)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x - ((go->maxHP - go->currentHP) * (1.0f/go->maxHP) * 5.0f), go->pos.y + go->scale.y + 3, go->pos.z);
			modelStack.Scale(go->currentHP * (1.0f / go->maxHP) * 10, 3, 1);
			RenderMesh(meshList[GEO_HPBAR], false);
			modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)), 0, 0, 1); 
		modelStack.Scale(go->scale.x * 1.3, go->scale.y, go->scale.z);
		modelStack.Rotate(-40, 0, 0, 1);
		RenderMesh(meshList[GEO_BULLET], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ENEMY_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x * 2, go->scale.y, go->scale.z);
		modelStack.Rotate(90, 0, 0, 1);
		RenderMesh(meshList[GEO_ENEMY_BULLET], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOSS_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//modelStack.Rotate(-40, 0, 0, 1);
		RenderMesh(meshList[GEO_BOSS_BULLET], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BOSS_MISSILE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)), 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.Rotate(90, 0, 0, 1);
		RenderMesh(meshList[GEO_BOSS_MISSILE], true);
		modelStack.PopMatrix();

		//target on player ship
		modelStack.PushMatrix();
		modelStack.Translate(m_ship->pos.x, m_ship->pos.y, m_ship->pos.z + 1);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_TARGET], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_STANDARD_ENEMY:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)) + 130, 0, 0, 1); //+90 to make 0 degrees the positive x axis
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_STANDARD_ENEMY], true);
		modelStack.PopMatrix();

		if (go->currentHP != go->maxHP)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x - ((go->maxHP - go->currentHP) * (1.0f / go->maxHP) * 5.0f), go->pos.y + go->scale.y + 3, go->pos.z);
			modelStack.Scale(go->currentHP * (1.0f / go->maxHP) * 10, 3, 1);
			RenderMesh(meshList[GEO_HPBAR], false);
			modelStack.PopMatrix();
		}
		// Triangle Char
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_BOSS_MISSILE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_STATIONED_ENEMY:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)) + 90, 0, 0, 1); //+90 to make 0 degrees the positive x axis
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_STATIONED_ENEMY], true);
		modelStack.PopMatrix();

		if (go->currentHP != go->maxHP)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x - ((go->maxHP - go->currentHP) * (1.0f / go->maxHP) * 5.0f), go->pos.y + go->scale.y + 3, go->pos.z);
			modelStack.Scale(go->currentHP * (1.0f / go->maxHP) * 10, 3, 1);
			RenderMesh(meshList[GEO_HPBAR], false);
			modelStack.PopMatrix();
		}

		// Triangle Char
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_BOSS_MISSILE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_EXPLODING_ENEMY:
		//ship
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)) + 90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_EXPLODING_ENEMY], true);
		modelStack.PopMatrix();
		//homing warning line
		if (HomingState)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x + ((m_ship->pos.x - go->pos.x) * 0.5f), go->pos.y + ((m_ship->pos.y - go->pos.y) * 0.5f), 0);
			modelStack.Rotate(go->rotateZ, 0, 0, 1);
			modelStack.Scale((m_ship->pos - go->pos).Length(), 1, 1);
			RenderMesh(meshList[GEO_HOMINGWARNING], true);
			modelStack.PopMatrix();
		}
		//hp bar
		if (go->currentHP != go->maxHP)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x - ((go->maxHP - go->currentHP) * (1.0f / go->maxHP) * 5.0f), go->pos.y + go->scale.y + 3, go->pos.z);
			modelStack.Scale(go->currentHP * (1.0f / go->maxHP) * 10, 3, 1);
			RenderMesh(meshList[GEO_HPBAR], false);
			modelStack.PopMatrix();
		}
		// Triangle Char
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_BOSS_MISSILE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BUDDY:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)) + 90, 0, 0, 1); //+90 to make 0 degrees the positive x axis
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SHIP], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ATTACK_POWERUP:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)) + 90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ATTACKPOWERUP], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BUDDY_POWERUP:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)) + 90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BUDDYPOWERUP], true);
		modelStack.PopMatrix();
		break;
	default:
		break;
	}
}

void SceneLevel3::RenderButton(Button button)
{
	modelStack.PushMatrix();
	modelStack.Translate(button.GetMid().x, button.GetMid().y, 1);
	modelStack.Scale(button.GetLength(), button.GetHeight(), 1);
	RenderMesh(meshList[GEO_BUTTON], false);
	modelStack.PopMatrix();

	int textSize = 5;
	float textposx = button.GetMid().x - (button.GetName().length() * 0.5f * textSize * 0.4f);
	float textposy = button.GetMid().y - (textSize * 0.4f);
	RenderTextOnScreen(meshList[GEO_TEXT], button.GetName(), Color(0, 0, 0), textSize, textposx, textposy);
}

void SceneLevel3::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);

	std::ostringstream ss;
	
	// Background
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, -1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(meshList[GEO_BG], false);
	modelStack.PopMatrix();

	// Game State specific 
	switch (gameState)
	{
	case G_START:
		RenderTextOnScreen(meshList[GEO_TEXT], "Level 3", Color(0, 0, 1), 7, 76, 80);
		RenderTextOnScreen(meshList[GEO_TEXT], level_description, Color(1, 1, 1), 5, 35, 60, 45);
		RenderButton(startButton);
		break;
	case G_PLAYING:
		// Game Objects
		RenderGO(m_ship);
		RenderGO(m_boss);
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}
		// Lives and Score
		ss.str("");
		ss << "Score: " << m_score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 0, 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Lives: ", Color(1, 1, 1), 5, 3, 93);
		for (int i = 0; i < m_lives; i++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(23 + (i * 6), 95, 1);
			modelStack.Rotate(180, 0, 0, 1);
			modelStack.Scale(5, 5, 5);
			RenderMesh(meshList[GEO_LIVES], false);
			modelStack.PopMatrix();
		}
		// Boss HP
		modelStack.PushMatrix();
		modelStack.Translate((m_worldWidth * 0.6) - ((m_boss->maxHP - m_boss->currentHP) * (1.0f / m_boss->maxHP) * 45.0f), 94, 2);
		modelStack.Scale(m_boss->currentHP * (1.0f / m_boss->maxHP) * 86.5, 5, 1);
		RenderMesh(meshList[GEO_BOSSHP], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.6, 94, 3);
		modelStack.Scale(95, 7, 1);
		RenderMesh(meshList[GEO_EMPTYBAR], false);
		modelStack.PopMatrix();
		// Message List
		for (auto popup : messageList)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], popup->message, popup->col, 3, popup->pos.x, popup->pos.y);
		}
		break;
	case G_PAUSED:
		// Game Objects
		RenderGO(m_ship);
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "Paused", Color(0, 1, 0), 7, 76, 80);
		RenderButton(resumeButton);
		RenderButton(menuButton);
		break;
	case G_WON:
		// Game Objects
		RenderTextOnScreen(meshList[GEO_TEXT], "Level Cleared", Color(0, 1, 0), 7, 68, 74);
		RenderButton(menuButton);
		RenderGO(m_ship);
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}
		break;
	case G_LOST:
		RenderTextOnScreen(meshList[GEO_TEXT], "You died", Color(0, 1, 0), 5, 74, 70);
		RenderButton(restartButton);
		RenderButton(menuButton);
		// Game Objects
		RenderGO(m_ship);
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}
		break;
	default:
		break;
	}


	//On screen text

	// m_speed and fps
	ss.str("");
	ss.precision(3);
	ss << "Speed: " << m_speed << " FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 0, 4);

	// game name
	RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid Shooter", Color(1, 1, 1), 4, 0, 0);
}

int SceneLevel3::GetNumGOs(GameObject::GAMEOBJECT_TYPE type)
{
	int num = 0;
	for (auto go : m_goList)
	{
		if (go->active && go->type == type)
		{
			num++;
		}
	}
	return num;
}

int SceneLevel3::GetNumGOs(GameObject::GAMEOBJECT_TYPE type[], int arrSize)
{
	int num = 0;
	for (auto go : m_goList)
	{
		if (go->active)
		{
			int test = sizeof(type);
			for (int i = 0; i < arrSize; ++i)
			{
				if (go->type == type[i])
				{
					num++;
				}
			}
		}
	}
	return num;
}

GameObject* SceneLevel3::GetNearestGO(Vector3 pos, GameObject::GAMEOBJECT_TYPE type[], int arrSize)
{
	GameObject* nearest = nullptr;
	for (auto go : m_goList)
	{
		for (int index = 0; index < arrSize; index++)
		{
			if (go->active && go->type == type[index])
			{
				if (nearest == nullptr)
				{
					nearest = go;
				}
				else
				{
					float current = (pos - nearest->pos).Length();
					float newgo = (pos - go->pos).Length();
					if (newgo < current)
					{
						nearest = go;
					}
				}
				break;
			}
		}
	}

	return nearest;
	
}

bool SceneLevel3::CheckZero(double number)
{
	if (number - 0 < Math::EPSILON || 0 - number > Math::EPSILON)
	{
		return true;
	}
	return false;
}

bool SceneLevel3::CheckCollision(GameObject* go1, GameObject* go2)
{
	Vector3 dis = go1->pos - go2->pos;
	
	float disSquared = dis.LengthSquared();
	if (disSquared <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x))
	{
		return true;
	}
	return false;
}

void SceneLevel3::RestartLevel()
{
	for (auto go : m_goList)
	{
		go->active = false;
	}

	asteroidInterval = 0;
	bossStateInterval = BOSS_STATE_INTERVAL;
	bossMovementInterval = 0;

	BOSS_MOVEMENT_INTERVAL = 0;
	PLAYER_FIRERATE_INTERVAL = 0.2;
	EXPLODING_SHIP_SPEED = 0.1f;

	mouseClicked = false;
	mousePressed = true;

	m_lives = 3;
	m_score = 0;

	gameState = G_START;

	HomingState = false;

	numBullet = 1;

	m_buddy = nullptr;

	m_ship->active = true;
	m_ship->maxHP = 5;
	m_ship->currentHP = m_ship->maxHP;
	m_ship->scale.Set(5, 5, 5);
	m_ship->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
	m_ship->vel.SetZero();
	m_ship->mass = 1;
	m_ship->momentOfInertia = m_ship->mass * m_ship->scale.x * m_ship->scale.x;
	m_ship->angularVelocity = 0;
	m_ship->fireInterval = 0;

	m_boss->active = true;
	m_boss->maxHP = 1000;
	m_boss->currentHP = 1000;
	m_boss->scale.Set(10, 10, 10);
	m_boss->pos.Set(m_worldWidth * 0.75f, m_worldHeight * 0.5f, 0);
	m_boss->vel.SetZero();
	m_boss->fireInterval = 0;
	

	bossState = BS_DEFAULT;
}

bool SceneLevel3::CheckButtonClick(Button button)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	x /= (w / m_worldWidth);
	y = h - y; //flip y
	y /= (h / m_worldHeight); //convert to 133:100

	
	if (mouseClicked && button.CheckWithinButton(x, y))
	{
		return true;
	}
	return false;
}

Vector3 SceneLevel3::GetAvailablePos()
{
	Vector3 GOpos;
	GRID list[3];

	if (m_ship->pos.x < m_worldWidth * 0.5f) // LEFT
	{
		list[0] = TOPRIGHT;
		list[1] = BOTRIGHT;
		if (m_ship->pos.y < m_worldHeight * 0.5f) // BOT
		{
			list[2] = TOPLEFT;
		}
		else // TOP
		{
			list[2] = BOTLEFT;
		}
	}
	else // RIGHT
	{
		list[0] = TOPLEFT;
		list[1] = BOTLEFT;
		if (m_ship->pos.y < m_worldHeight * 0.5f) // BOT
		{
			list[2] = TOPRIGHT;
		}
		else // TOP
		{
			list[2] = BOTRIGHT;
		}
	}

	GRID chosenGrid;
	switch (Math::RandIntMinMax(1, 3))
	{
	case 1:
		chosenGrid = list[0];
		break;
	case 2:
		chosenGrid = list[1];
		break;
	case 3:
		chosenGrid = list[2];
		break;
	default:
		break;
	}

	switch (chosenGrid)
	{
	case TOPLEFT:
		GOpos.Set(Math::RandFloatMinMax(0, m_worldWidth * 0.5f), Math::RandFloatMinMax(m_worldHeight * 0.5f, m_worldHeight), 0);
		break;
	case TOPRIGHT:
		GOpos.Set(Math::RandFloatMinMax(m_worldWidth * 0.5f, m_worldWidth), Math::RandFloatMinMax(m_worldHeight * 0.5f, m_worldHeight), 0);
		break;
	case BOTLEFT:
		GOpos.Set(Math::RandFloatMinMax(0, m_worldWidth * 0.5f), Math::RandFloatMinMax(0, m_worldHeight * 0.5f), 0);
		break;
	case BOTRIGHT:
		GOpos.Set(Math::RandFloatMinMax(m_worldWidth * 0.5f, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight * 0.5f), 0);
		break;
	}

	return GOpos;
}

void SceneLevel3::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}

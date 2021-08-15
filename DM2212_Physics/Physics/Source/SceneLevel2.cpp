#include "SceneLevel2.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "MeshBuilder.h"
#include "LoadTGA.h"

SceneLevel2::SceneLevel2()
{
}

SceneLevel2::~SceneLevel2()
{
}

void SceneLevel2::Init()
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
	meshList[GEO_BG]->textureID = LoadTGA("Image/spacebackground2.tga");

	meshList[GEO_SHIP] = MeshBuilder::GenerateQuad("spaceship", Color(1,1,1), 2.0f);
	meshList[GEO_SHIP]->textureID = LoadTGA("Image/spaceship.tga");
	meshList[GEO_ASTEROID] = MeshBuilder::GenerateQuad("asteroid", Color(1, 1, 1), 2.0f);
	meshList[GEO_ASTEROID]->textureID = LoadTGA("Image/asteroid.tga");
	meshList[GEO_EXPLODING_ENEMY] = MeshBuilder::GenerateQuad("exploding enemy ship", Color(0, 0, 0), 2.0f);
	meshList[GEO_EXPLODING_ENEMY]->textureID = LoadTGA("Image/exploding_ship.tga");
	meshList[GEO_STANDARD_ENEMY] = MeshBuilder::GenerateQuad("standard ship", Color(0, 0, 0), 2.0f);
	meshList[GEO_STANDARD_ENEMY]->textureID = LoadTGA("Image/standardenemy.tga");
	meshList[GEO_STATIONED_ENEMY] = MeshBuilder::GenerateQuad("stationary ship", Color(0, 0, 0), 2.0f);
	meshList[GEO_STATIONED_ENEMY]->textureID = LoadTGA("Image/stationedspaceship.tga");

	meshList[GEO_BULLET] = MeshBuilder::GenerateQuad("bullet", Color(1, 1, 0), 2.0f);
//	meshList[GEO_BULLET]->textureID = LoadTGA("Image/kanae.tga"); //set when player chooses a side
	meshList[GEO_ENEMY_BULLET] = MeshBuilder::GenerateQuad("enemy bullet", Color(1, 1, 0), 2.0f);
//	meshList[GEO_ENEMY_BULLET]->textureID = LoadTGA("Image/misumi.tga"); //set when player chooses a side

	meshList[GEO_HPBAR] = MeshBuilder::GenerateQuad("hp bar", Color(1, 1, 1), 1.0f);
	meshList[GEO_HPBAR]->textureID = LoadTGA("Image/hpbar.tga");
	meshList[GEO_LIVES] = MeshBuilder::GenerateQuad("lives icon", Color(1, 1, 1), 1.0f);
	meshList[GEO_LIVES]->textureID = LoadTGA("Image/lives.tga");
	meshList[GEO_HOMINGWARNING] = MeshBuilder::GenerateQuad("homing warning line", Color(1, 1, 1), 1.0f);
	meshList[GEO_HOMINGWARNING]->textureID = LoadTGA("Image/homingline.tga");
	meshList[GEO_TIMEOUT] = MeshBuilder::GenerateQuad("timeout bg", Color(1, 1, 1), 1.0f);
	meshList[GEO_TIMEOUT]->textureID = LoadTGA("Image/timeout.tga");
	meshList[GEO_REDBANNER] = MeshBuilder::GenerateQuad("enemy score banner", Color(1, 1, 1), 1.0f);
	meshList[GEO_REDBANNER]->textureID = LoadTGA("Image/redscorebanner.tga");
	meshList[GEO_BLUEBANNER] = MeshBuilder::GenerateQuad("ally score banner", Color(1, 1, 1), 1.0f);
	meshList[GEO_BLUEBANNER]->textureID = LoadTGA("Image/bluescorebanner.tga");

	meshList[GEO_CURRY2D] = MeshBuilder::GenerateQuad("curry 2d", Color(1, 1, 1), 2.0f);
	meshList[GEO_CURRY2D]->textureID = LoadTGA("Image/curry2d.tga");
	meshList[GEO_OMURICE2D] = MeshBuilder::GenerateQuad("omurice 2d", Color(1, 1, 1), 2.0f);
	meshList[GEO_OMURICE2D]->textureID = LoadTGA("Image/omurice2d.tga");

	meshList[GEO_YOU] = MeshBuilder::GenerateQuad("you label", Color(1, 1, 1), 2.0f);
	meshList[GEO_YOU]->textureID = LoadTGA("Image/you.tga");
	meshList[GEO_KANAE] = MeshBuilder::GenerateQuad("KANAE", Color(1, 1, 1), 2.0f);
	meshList[GEO_KANAE]->textureID = LoadTGA("Image/kanaehead.tga");
	meshList[GEO_MISUMI] = MeshBuilder::GenerateQuad("MISUMI", Color(1, 1, 1), 2.0f);
	meshList[GEO_MISUMI]->textureID = LoadTGA("Image/misumihead.tga");

	// Creation of Game Objects to be used
	for (int i = 0; i < 200; i++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}

	// Setting of buttons
	startButton.Set("Start", Vector3(m_worldWidth / 2, m_worldHeight * 0.2f, 0), 30, 10);
	menuButton.Set("Main Menu", Vector3(m_worldWidth / 2, 30, 0), 30, 10);
	resumeButton.Set("Resume", Vector3(m_worldWidth / 2, 45, 0), 30, 10);
	nextButton.Set("Next Level", Vector3(m_worldWidth / 2, 30, 0), 30, 10);
	restartButton.Set("Restart", Vector3(m_worldWidth / 2, 45, 0), 30, 10);

	omuriceButton.Set("OMURICE", Vector3(m_worldWidth * 0.33f, m_worldHeight * 0.1f, 0), 30, 10);
	curryButton.Set("CURRY", Vector3(m_worldWidth * 0.67f, m_worldHeight * 0.1f, 0), 30, 10);
	

	// Setting of vars
	mouseClicked = false;
	mousePressed = false;

	// Init/Setting of  game related vars
	gameState = G_STORY;
	remainingTime = 30;
	level_description = " ";

	enemyInterval = 0;
	asteroidInterval = 0;

	EXPLODING_SHIP_SPEED = 0.1f;
	PLAYER_FIRERATE_INTERVAL = 0.5f;

	m_allyscore = 0;
	m_enemyscore = 0;

	HomingState = false;
	explodingTarget = nullptr;

	numBullet = 1;

	allyFlag = enemyFlag = GameObject::GO_CURRY2D;

	// Construct player ships of all teams and set stats
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

	m_explodingship = new GameObject(GameObject::GO_EXPLODING_ENEMY);
	m_explodingship->active = true;
	m_explodingship->maxHP = 5;
	m_explodingship->currentHP = m_explodingship->maxHP;
	m_explodingship->scale.Set(4, 4, 4);
	m_explodingship->pos.Set(m_worldWidth * 0.75f, m_worldHeight * 0.25f, 0);
	m_explodingship->vel.Set(-1, 0, 0);
	m_explodingship->fireInterval = 0;

	m_standardship = new GameObject(GameObject::GO_STANDARD_ENEMY);
	m_standardship->active = true;
	m_standardship->maxHP = 5;
	m_standardship->currentHP = m_standardship->maxHP;
	m_standardship->scale.Set(6, 6, 6);
	m_standardship->pos.Set(m_worldWidth * 0.75f, m_worldHeight * 0.5f, 0);
	m_standardship->vel.Set(-1, 0, 0);
	m_standardship->fireInterval = 0;

	m_stationedship = new GameObject(GameObject::GO_STATIONED_ENEMY);
	m_stationedship->active = true;
	m_stationedship->maxHP = 5;
	m_stationedship->currentHP = m_stationedship->maxHP;
	m_stationedship->scale.Set(6, 6, 6);
	m_stationedship->pos.Set(m_worldWidth * 0.75f, m_worldHeight * 0.75f, 0);
	m_stationedship->vel.Set(-1, 0, 0);
	m_stationedship->fireInterval = 0;

	m_buddy1 = new GameObject(GameObject::GO_BUDDY);
	m_buddy1->active = true;
	m_buddy1->scale.Set(4, 4, 4);
	m_buddy1->maxHP = 5;
	m_buddy1->currentHP = m_buddy1->maxHP;
	m_buddy1->pos.Set(m_worldWidth * 0.25, m_worldHeight * 0.25f, 1);
	m_buddy1->vel.Set(1, 0, 0);
	m_buddy1->fireInterval = 0;
	
	m_buddy2 = new GameObject(GameObject::GO_BUDDY);
	m_buddy2->active = true;
	m_buddy2->scale.Set(4, 4, 4);
	m_buddy2->maxHP = 5;
	m_buddy2->currentHP = m_buddy1->maxHP;
	m_buddy2->pos.Set(m_worldWidth * 0.25, m_worldHeight * 0.75f, 1);
	m_buddy2->vel.Set(1, 0, 0);
	m_buddy2->fireInterval = 0;

	m_shipList.push_back(m_ship);
	m_shipList.push_back(m_buddy1);
	m_shipList.push_back(m_buddy2);
	m_shipList.push_back(m_explodingship);
	m_shipList.push_back(m_standardship);
	m_shipList.push_back(m_stationedship);
}

GameObject* SceneLevel2::FetchGO()
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

GameObject* SceneLevel2::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	// Fetch an active game object of a spcific type from m_goList and return it
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active && go->type == type)
		{
			return go;
		}
	}
	return nullptr;
}

void SceneLevel2::Update(double dt)
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
	
	// Settings/Checks based on game state
	switch (gameState)
	{
	case G_STORY:
		if (CheckButtonClick(curryButton))
		{
			gameState = G_START;
			allyFlag = GameObject::GO_CURRY2D;
			enemyFlag = GameObject::GO_OMURICE2D;
			allyChar = meshList[GEO_MISUMI];
			enemyChar = meshList[GEO_KANAE];
			meshList[GEO_BULLET]->textureID = LoadTGA("Image/misumi.tga");
			meshList[GEO_ENEMY_BULLET]->textureID = LoadTGA("Image/kanae.tga");
			level_description = "3v3 Team Cooking Battle In Space! You have chosen to side with Team Curry. Shoot asteroids to turn them into Japanese Curry Rice! Crashing into ships or asteroids are okay in this round, only bullets will deduct HP, running out of HP will put ships into a 5 second timeout. Shoot asteroids or Kill enemies to earn points. Team with higher number of points after 30 seconds wins!";
		}
		else if (CheckButtonClick(omuriceButton))
		{
			gameState = G_START;
			allyFlag = GameObject::GO_OMURICE2D;
			enemyFlag = GameObject::GO_CURRY2D;
			enemyChar = meshList[GEO_MISUMI];
			allyChar = meshList[GEO_KANAE];
			meshList[GEO_BULLET]->textureID = LoadTGA("Image/kanae.tga");
			meshList[GEO_ENEMY_BULLET]->textureID = LoadTGA("Image/misumi.tga");
			level_description = "3v3 Team Cooking Battle In Space! You have chosen to side with Team Omurice. Shoot asteroids to turn them into Omurice! Crashing into ships or asteroids are okay in this round, only bullets will deduct HP, running out of HP will put ships into a 5 second timeout. Shoot asteroids or Kill enemies to earn points. Team with higher number of points after 30 seconds wins!";
		}
		break;
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
		if (CheckButtonClick(nextButton))
		{
			RestartLevel();
			Application::SwitchScene(Application::S_LEVEL3);
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

void SceneLevel2::UpdateGame(double dt)
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
	if (asteroidInterval >= 0.75 && GetNumGOs(GameObject::GO_ASTEROID) < 10)
	{
		asteroidInterval = 0;
		GameObject* go = FetchGO();
		go->active = true;
		go->type = GameObject::GO_ASTEROID;
		go->maxHP = Math::RandIntMinMax(1, 3);
		go->scale.Set(go->maxHP + 2, go->maxHP + 2, 1);
		go->pos = GetAvailablePos();
		go->vel.Set(Math::RandFloatMinMax(-5, 5), Math::RandFloatMinMax(-5, 5), 0);
	}
	else
	{
		asteroidInterval += dt;
	}
	

	// ship updates
	for (auto go : m_shipList)
	{
		if (go->timeout > 0)
		{
			go->timeout -= dt;
			if (go->timeout < 0)
			{
				go->active = true;
			}
		}
		else
		{
			UpdateGO(go, dt);
		}
	}

	// all other GO updates
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			UpdateGO(go, dt);
		}
	}

	remainingTime -= dt;
	// win lose condition
	if (remainingTime <= 0)
	{
		if (m_allyscore > m_enemyscore)
		{
			gameState = G_WON;
		}
		else
		{
			gameState = G_LOST;
		}
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

void SceneLevel2::UpdateGO(GameObject* go, double dt)
{
	//move gos based on vel
	go->pos += go->vel * dt * m_speed;


	switch (go->type)
	{
	case GameObject::GO_SHIP:
		UpdatePlayerShip(dt);
		break;
	case GameObject::GO_ASTEROID:
		UpdateAsteroid(go);
		break;
	case GameObject::GO_BULLET:
		UpdateBullet(go);
		break;
	case GameObject::GO_ENEMY_BULLET:
		UpdateEnemyBullet(go);
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
	default:
		break;
	}
}

void SceneLevel2::UpdatePlayerShip(double dt)
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
	// Increase and decrease mass of ship
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
	//m_ship->pos += m_ship->vel * dt * m_speed;

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

void SceneLevel2::UpdateBuddyShip(GameObject* go, double dt)
{
	// Warp screen
	go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x, m_worldWidth + go->scale.x);
	go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y, m_worldHeight + go->scale.y);

	int distance;
	//just so the 2 buddy ships dont stick tgt
	if (go == m_buddy1)
	{
		distance = 50;
	}
	else
	{
		distance = 100;
	}
	// follow player
	if ((go->pos - m_ship->pos).Length() > distance)
	{
		go->vel = (m_ship->pos - go->pos).Normalized() * BUDDY_SHIP_SPEED;
	}
	// fire bullet
	GameObject* target = GetNearestGO(go->pos, GameObject::GO_ASTEROID);
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

void SceneLevel2::UpdateBullet(GameObject* go)
{
	// Unspawn bullets when they leave screen
	if (go->pos.x >= m_worldWidth + go->scale.x
		|| go->pos.x + go->scale.x <= 0
		|| go->pos.y >= m_worldHeight + go->scale.y
		|| go->pos.y + go->scale.y <= 0)
	{
		go->active = false;
	}

	// Collision with ships
	if (CheckCollision(go, m_explodingship))
	{
		go->active = false;
		m_explodingship->currentHP--;
		if (m_explodingship->currentHP <= 0)
		{
			m_allyscore += 50;
			messageList.push_back(new PopUp("+50", Vector3(m_explodingship->pos.x, m_explodingship->pos.y + m_explodingship->scale.y, m_explodingship->pos.z + 1), Color(0.5, 0.5, 1), 1));
			m_explodingship->timeout = 5;
			m_explodingship->currentHP = m_explodingship->maxHP;
			m_explodingship->active = false;
		}
	}
	else if (CheckCollision(go, m_stationedship))
	{
		go->active = false;
		m_stationedship->currentHP--;
		if (m_stationedship->currentHP <= 0)
		{
			m_allyscore += 50;
			messageList.push_back(new PopUp("+50", Vector3(m_stationedship->pos.x, m_stationedship->pos.y + m_stationedship->scale.y, m_stationedship->pos.z + 1), Color(0.5, 0.5, 1), 1));
			m_stationedship->timeout = 5;
			m_stationedship->currentHP = m_stationedship->maxHP;
			m_stationedship->active = false;
		}
	}
	else if (CheckCollision(go, m_standardship))
	{
		go->active = false;
		m_standardship->currentHP--;
		if (m_standardship->currentHP <= 0)
		{
			m_allyscore += 50;
			messageList.push_back(new PopUp("+50", Vector3(m_standardship->pos.x, m_standardship->pos.y + m_standardship->scale.y, m_standardship->pos.z + 1), Color(0.5, 0.5, 1), 1));
			m_standardship->timeout = 5;
			m_standardship->currentHP = m_standardship->maxHP;
			m_standardship->active = false;
		}
	}
}

void SceneLevel2::UpdateEnemyBullet(GameObject* go)
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
		if (m_ship->currentHP <= 0)
		{
			m_enemyscore += 50;
			messageList.push_back(new PopUp("+50", Vector3(m_ship->pos.x, m_ship->pos.y + m_ship->scale.y, m_ship->pos.z + 1), Color(1, 0.5, 0.5), 1));
			m_ship->timeout = 5;
			m_ship->currentHP = m_ship->maxHP;
			m_ship->active = false;
		}
	}
	// Collision with buddy ships
	else if (CheckCollision(go, m_buddy1))
	{
		go->active = false;
		m_buddy1->currentHP--;
		if (m_buddy1->currentHP <= 0)
		{
			m_enemyscore += 50;
			messageList.push_back(new PopUp("+50", Vector3(m_buddy1->pos.x, m_buddy1->pos.y + m_buddy1->scale.y, m_buddy1->pos.z + 1), Color(1, 0.5, 0.5), 1));
			m_buddy1->timeout = 5;
			m_buddy1->currentHP = m_buddy1->maxHP;
			m_buddy1->active = false;
		}
	}
	else if (CheckCollision(go, m_buddy2))
	{
		go->active = false;
		m_buddy2->currentHP--;
		if (m_buddy2->currentHP <= 0)
		{
			m_enemyscore += 50;
			messageList.push_back(new PopUp("+50", Vector3(m_buddy2->pos.x, m_buddy2->pos.y + m_buddy2->scale.y, m_buddy2->pos.z + 1), Color(1, 0.5, 0.5), 1));
			m_buddy2->timeout = 5;
			m_buddy2->currentHP = m_buddy2->maxHP;
			m_buddy2->active = false;
		}
	}
	
}

void SceneLevel2::UpdateAsteroid(GameObject* go)
{
	// Warp screen
	go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x, m_worldWidth + go->scale.x);
	go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y, m_worldHeight + go->scale.y);
	
	for (auto go2 : m_goList)
	{
		if (go2->active && CheckCollision(go, go2))
		{
			if (go2->type == GameObject::GO_BULLET)
			{
				go->active = false;
				go2->active = false;
				m_allyscore += go->maxHP * 10;
				std::ostringstream score;
				score << "+" << go->maxHP * 10;
				messageList.push_back(new PopUp(score.str(), Vector3(go->pos.x, go->pos.y + go->scale.y, go->pos.z + 1), Color(0, 0, 1), 1));
				GameObject* newForm = FetchGO();
				newForm->type = allyFlag;
				newForm->active = true;
				newForm->pos = go->pos;
				newForm->vel = go->vel * 0.5f;
				newForm->scale = go->scale;
				break;
			
			}
			else if (go2->type == GameObject::GO_ENEMY_BULLET)
			{
				go->active = false;
				go2->active = false;
				m_enemyscore += go->maxHP * 10;
				std::ostringstream score;
				score << "+" << go->maxHP * 10;
				messageList.push_back(new PopUp(score.str(), Vector3(go->pos.x, go->pos.y + go->scale.y, go->pos.z + 1), Color(1, 0, 0), 1));
				GameObject* newForm = FetchGO();
				newForm->type = enemyFlag;
				newForm->active = true;
				newForm->pos = go->pos;
				newForm->vel = go->vel * 0.5f;
				newForm->scale = go->scale;
				break;
			}
			
		}
	}

	if (CheckCollision(go, m_explodingship))
	{
		EXPLODING_SHIP_SPEED = 0.1f;
		go->active = false;
		m_enemyscore += go->maxHP * 10;
		std::ostringstream score;
		score << "+" << go->maxHP * 10;
		messageList.push_back(new PopUp(score.str(), Vector3(go->pos.x, go->pos.y + go->scale.y, go->pos.z + 1), Color(1, 0, 0), 1));
		GameObject* newForm = FetchGO();
		newForm->type = enemyFlag;
		newForm->active = true;
		newForm->pos = go->pos;
		newForm->vel = go->vel * 0.5f;
		newForm->scale = go->scale;
	}
}

void SceneLevel2::UpdateStandardEnemy(GameObject* go, double dt)
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
	// Change direction
	if (Math::RandIntMinMax(1, 100) == 24)
	{
		go->vel.Set(Math::RandFloatMinMax(-1, 1), Math::RandFloatMinMax(-1, 1), 0);
		go->vel = go->vel.Normalized() * STANDARD_ENEMY_SHIP_SPEED;
	}

}

void SceneLevel2::UpdateStationedEnemy(GameObject* go, double dt)
{
	// Warp screen
	go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x, m_worldWidth + go->scale.x);
	go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y, m_worldHeight + go->scale.y);

	GameObject* target = GetNearestGO(go->pos, GameObject::GO_ASTEROID);
	if (go->fireInterval >= ENEMY_FIRERATE_INTERVAL && target != nullptr)
	{
		go->fireInterval = 0;
		go->dir = target->pos - go->pos;

		GameObject* bullet = FetchGO();
		bullet->active = true;
		bullet->type = GameObject::GO_ENEMY_BULLET;
		bullet->scale.Set(1, 1, 1);
		bullet->pos = go->pos;
		bullet->vel = (target->pos - go->pos).Normalized() * ENEMY_BULLET_SPEED;

	}
	else
	{
		go->fireInterval += dt;
	}

}

void SceneLevel2::UpdateExplodingEnemy(GameObject* go, double dt)
{
	// Warp screen
	go->pos.x = Math::Wrap(go->pos.x, 0 - go->scale.x, m_worldWidth + go->scale.x);
	go->pos.y = Math::Wrap(go->pos.y, 0 - go->scale.y, m_worldHeight + go->scale.y);

	explodingTarget = FetchGO(GameObject::GO_ASTEROID);
	if (explodingTarget != nullptr)
	{
		HomingState = true;
		EXPLODING_SHIP_SPEED += 2 * dt;
		go->vel = (explodingTarget->pos - go->pos).Normalized() * EXPLODING_SHIP_SPEED;
		go->rotateZ = Math::RadianToDegree(atan2(go->vel.y, go->vel.x));
	}
	else
	{
		HomingState = false;
		go->vel.SetZero();
	}
	

}

void SceneLevel2::RenderGO(GameObject *go)
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
		
		//char
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(3, 3, 3);
		RenderMesh(meshList[GEO_YOU], false);
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
		modelStack.Scale(go->scale.x * 2, go->scale.y, go->scale.z);
		modelStack.Rotate(90, 0, 0, 1);
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

		//char
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(3, 3, 3);
		RenderMesh(enemyChar, false);
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

		//char
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(3, 3, 3);
		RenderMesh(enemyChar, false);
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
			modelStack.Translate(go->pos.x + ((explodingTarget->pos.x - go->pos.x) * 0.5f), go->pos.y + ((explodingTarget->pos.y - go->pos.y) * 0.5f), 0);
			modelStack.Rotate(go->rotateZ, 0, 0, 1);
			modelStack.Scale((explodingTarget->pos - go->pos).Length(), 1, 1);
			RenderMesh(meshList[GEO_HOMINGWARNING], true);
			modelStack.PopMatrix();
		}
		
		if (go->currentHP != go->maxHP)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x - ((go->maxHP - go->currentHP) * (1.0f / go->maxHP) * 5.0f), go->pos.y + go->scale.y + 3, go->pos.z);
			modelStack.Scale(go->currentHP * (1.0f / go->maxHP) * 10, 3, 1);
			RenderMesh(meshList[GEO_HPBAR], false);
			modelStack.PopMatrix();
		}
		//char
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(3, 3, 3);
		RenderMesh(enemyChar, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BUDDY:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(go->dir.y, go->dir.x)) + 90, 0, 0, 1); //+90 to make 0 degrees the positive x axis
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SHIP], true);
		modelStack.PopMatrix();
		
		if (go->currentHP != go->maxHP)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x - ((go->maxHP - go->currentHP) * (1.0f / go->maxHP) * 5.0f), go->pos.y + go->scale.y + 3, go->pos.z);
			modelStack.Scale(go->currentHP * (1.0f / go->maxHP) * 10, 3, 1);
			RenderMesh(meshList[GEO_HPBAR], false);
			modelStack.PopMatrix();
		}
		//char
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 1);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(3, 3, 3);
		RenderMesh(allyChar, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CURRY2D:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)) + 90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CURRY2D], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_CURRY3D:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)) + 90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_CURRY3D], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_OMURICE2D:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)) + 90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_OMURICE2D], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_OMURICE3D:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(go->vel.y, go->vel.x)) + 90, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_OMURICE3D], true);
		modelStack.PopMatrix();
		break;
	default:
		break;
	}
}

void SceneLevel2::RenderButton(Button button)
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

void SceneLevel2::RenderShips()
{
	for (auto go : m_shipList)
	{
		if (go->active)
		{
			RenderGO(go);
		}
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(10, 10, 10);
			RenderMesh(meshList[GEO_TIMEOUT], false);
			modelStack.PopMatrix();

			std::ostringstream ss;
			ss << (int)go->timeout;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 5, go->pos.x - 1, go->pos.y - 2);
		}
	}
}

void SceneLevel2::Render()
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
	case G_STORY:
		//title
		RenderTextOnScreen(meshList[GEO_TEXT], "Pick a side", Color(0, 0, 1), 7, 74, 80);
		//kanae photo
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.2, m_worldHeight * 0.7, 0);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_KANAE], false);
		modelStack.PopMatrix();
		//omurice text
		RenderTextOnScreen(meshList[GEO_TEXT], "\"Omurice is better than japanese curry rice !!!\"", 
			Color(1, 1, 1), 5, m_worldWidth * 0.2 + 15, m_worldHeight * 0.7, 30);
		//misumi photo
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.8, m_worldHeight * 0.4, 0);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_MISUMI], false);
		modelStack.PopMatrix();
		//curry text
		RenderTextOnScreen(meshList[GEO_TEXT], "\"No! Curry is superior!\"", 
			Color(1, 1, 1), 5, m_worldWidth * 0.2f + 15, m_worldHeight * 0.4, 30);
		//buttons
		RenderButton(curryButton);
		RenderButton(omuriceButton);
		break;
	case G_START:
		RenderTextOnScreen(meshList[GEO_TEXT], "Level 2", Color(0, 0, 1), 7, 76, 80);
		RenderTextOnScreen(meshList[GEO_TEXT], level_description, Color(1, 1, 1), 5, 30, 70, 50);
		RenderButton(startButton);
		break;
	case G_PLAYING:
		// Game Objects
		RenderShips();
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}
		//scoreboard
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.5 - 12, m_worldHeight * 0.95, 1);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(20, 10, 10);
		RenderMesh(meshList[GEO_BLUEBANNER], false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(m_worldWidth * 0.5 + 12, m_worldHeight * 0.95, 1);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(20, 10, 10);
		RenderMesh(meshList[GEO_REDBANNER], false);
		modelStack.PopMatrix();
		ss.str(" ");
		ss << m_allyscore;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.2, 0.2, 1), 5, m_worldWidth * 0.5 - 10 - (ss.str().length() * 2), 92.5);
		ss.str(" ");
		ss << m_enemyscore;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0.2, 0.2), 5, m_worldWidth * 0.5 + 14 - (ss.str().length() * 2), 92.5);
		ss.str(" ");
		//ss.precision(2);
		ss << (int)remainingTime;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 5, m_worldWidth * 0.5f - (ss.str().length() * 1) , 93);
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
		RenderGO(m_ship);
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				RenderGO(go);
			}
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "You Won!", Color(0, 1, 0), 7, 74, 70);
		RenderButton(nextButton);
		break;
	case G_LOST:
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
		RenderTextOnScreen(meshList[GEO_TEXT], "You Lost.", Color(1, 0, 0), 7, 74, 70);
		RenderButton(restartButton);
		RenderButton(menuButton);
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

int SceneLevel2::GetNumGOs(GameObject::GAMEOBJECT_TYPE type)
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

int SceneLevel2::GetNumGOs(GameObject::GAMEOBJECT_TYPE type[], int arrSize)
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

GameObject* SceneLevel2::GetNearestGO(Vector3 pos, GameObject::GAMEOBJECT_TYPE type)
{
	GameObject* nearest = nullptr;
	for (auto go : m_goList)
	{
		
		if (go->active && go->type == type)
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

	return nearest;
	
}

bool SceneLevel2::CheckZero(double number)
{
	if (number - 0 < Math::EPSILON || 0 - number > Math::EPSILON)
	{
		return true;
	}
	return false;
}

bool SceneLevel2::CheckCollision(GameObject* go1, GameObject* go2)
{
	Vector3 dis = go1->pos - go2->pos;
	
	float disSquared = dis.LengthSquared();
	if (disSquared <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x))
	{
		return true;
	}
	return false;
}

void SceneLevel2::RestartLevel()
{
	for (auto go : m_goList)
	{
		go->active = false;
	}
	// Setting of vars
	mouseClicked = false;
	mousePressed = false;

	// Init/Setting of  game related vars
	gameState = G_STORY;
	remainingTime = 30;
	level_description = " ";

	enemyInterval = 0;
	asteroidInterval = 0;

	EXPLODING_SHIP_SPEED = 0.1f;
	PLAYER_FIRERATE_INTERVAL = 0.5f;

	m_allyscore = 0;
	m_enemyscore = 0;

	HomingState = false;
	explodingTarget = nullptr;

	numBullet = 1;

	allyFlag = enemyFlag = GameObject::GO_CURRY2D;

	// Construct m_ship, setting of player/m_ship stats etc
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

	m_explodingship->active = true;
	m_explodingship->maxHP = 5;
	m_explodingship->currentHP = m_explodingship->maxHP;
	m_explodingship->scale.Set(4, 4, 4);
	m_explodingship->pos.Set(m_worldWidth * 0.75f, m_worldHeight * 0.25f, 0);
	m_explodingship->vel.Set(-1, 0, 0);
	m_explodingship->fireInterval = 0;

	m_standardship->active = true;
	m_standardship->maxHP = 5;
	m_standardship->currentHP = m_standardship->maxHP;
	m_standardship->scale.Set(6, 6, 6);
	m_standardship->pos.Set(m_worldWidth * 0.75f, m_worldHeight * 0.5f, 0);
	m_standardship->vel.Set(-1, 0, 0);
	m_standardship->fireInterval = 0;

	m_stationedship->active = true;
	m_stationedship->maxHP = 5;
	m_stationedship->currentHP = m_stationedship->maxHP;
	m_stationedship->scale.Set(6, 6, 6);
	m_stationedship->pos.Set(m_worldWidth * 0.75f, m_worldHeight * 0.75f, 0);
	m_stationedship->vel.Set(-1, 0, 0);
	m_stationedship->fireInterval = 0;

	m_buddy1->active = true;
	m_buddy1->scale.Set(4, 4, 4);
	m_buddy1->maxHP = 5;
	m_buddy1->currentHP = m_buddy1->maxHP;
	m_buddy1->pos.Set(m_worldWidth * 0.25, m_worldHeight * 0.25f, 1);
	m_buddy1->vel.Set(1, 0, 0);
	m_buddy1->fireInterval = 0;

	m_buddy2->active = true;
	m_buddy2->scale.Set(4, 4, 4);
	m_buddy2->maxHP = 5;
	m_buddy2->currentHP = m_buddy1->maxHP;
	m_buddy2->pos.Set(m_worldWidth * 0.25, m_worldHeight * 0.75f, 1);
	m_buddy2->vel.Set(1, 0, 0);
	m_buddy2->fireInterval = 0;

}

bool SceneLevel2::CheckButtonClick(Button button)
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

Vector3 SceneLevel2::GetAvailablePos()
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

void SceneLevel2::Exit()
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

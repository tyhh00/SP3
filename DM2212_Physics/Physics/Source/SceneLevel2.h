#ifndef SCENE_LEVEL2_H
#define SCENE_LEVEL2_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Button.h"

class SceneLevel2 : public SceneBase
{
	static const int MAX_SPEED = 20;
	static const int BULLET_SPEED = 70;
	static const int ENEMY_BULLET_SPEED = 50;
	//static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;
	static const int MAX_ANGULAR_SPEED = 1;
	static const int STANDARD_ENEMY_SHIP_SPEED = 5;
	static const int BUDDY_SHIP_SPEED = 15;
	static const int ENEMY_FIRERATE_INTERVAL = 2;
	static const int BUDDY_FIRERATE_INTERVAL = 3;

public:
	SceneLevel2();
	~SceneLevel2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go); // Render GO based on their type
	void RenderButton(Button button); // Render Button
	void RenderShips(); // Render Ships
	void UpdateGame(double dt); // Game Related Updates
	void UpdateGO(GameObject* go, double dt); // Updates GO based on type
	void UpdatePlayerShip(double dt); // Player Ship Updates
	void UpdateBuddyShip(GameObject* go, double dt); // Buddy Ship Updates
	void UpdateBullet(GameObject* go); // Bullet Updates
	void UpdateEnemyBullet(GameObject* go); // Enemy Bullet Updates
	void UpdateAsteroid(GameObject* go); // Asteroid Updates
	void UpdateStandardEnemy(GameObject* go, double dt); // Standard Enemy Ship Updates
	void UpdateStationedEnemy(GameObject* go, double dt); // Stationed Enemy Ship Updates
	void UpdateExplodingEnemy(GameObject* go, double dt); // Exploding Enemy Ship Updates
	bool CheckCollision(GameObject* go1, GameObject* go2); // Check for collision between 2 GOs
	bool CheckZero(double number); // Checks if a number is zero [EPSILON things]
	int GetNumGOs(GameObject::GAMEOBJECT_TYPE type); // Gets number of active GOs of certain type
	int GetNumGOs(GameObject::GAMEOBJECT_TYPE type[], int arrSize); // Gets number of active GOs of various types
	Vector3 GetAvailablePos(); // Get Available Pos based on Player's pos [GRID]
	GameObject* GetNearestGO(Vector3 pos, GameObject::GAMEOBJECT_TYPE type); 
	void RestartLevel();
	bool CheckButtonClick(Button button);

	GameObject* FetchGO();
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);
protected:

	// Grid 
	enum GRID
	{
		TOPLEFT,
		TOPRIGHT,
		BOTLEFT,
		BOTRIGHT,

		GRID_TOTAL
	};

	// Game States
	enum GAME_STATE
	{
		G_STORY,
		G_START,
		G_PLAYING,
		G_PAUSED,
		G_WON,
		G_LOST,
		GS_TOTAL
	};

	// For warning/messages that appear on screen
	struct PopUp
	{
		double time;
		Vector3 pos;
		Color col;
		std::string message;

		PopUp() : time(0), message(" "), pos(0, 0, 0), col(1, 1, 1)
		{

		}

		PopUp(std::string message, Vector3 pos, Color col, double time)
		{
			this->message = message;
			this->pos = pos;
			this->col = col;
			this->time = time;
		}
	};

	// Mouse Click
	bool mousePressed;
	bool mouseClicked;

	// Buttons
	Button startButton;
	Button resumeButton;
	Button menuButton;
	Button nextButton;
	Button restartButton;
	Button curryButton, omuriceButton;

	// Physics
	std::vector<GameObject *> m_goList;
	Vector3 m_force;
	Vector3 m_torque;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;

	// Game Related
	int m_allyscore;
	int m_enemyscore;
	double remainingTime;
	std::string level_description;
	std::vector<PopUp*> messageList;
	GAME_STATE gameState;

	// Unique/Single Game Objects
	GameObject* m_ship;
	GameObject* m_explodingship;
	GameObject* m_standardship;
	GameObject* m_stationedship;
	GameObject* m_buddy1, *m_buddy2;
	
	GameObject* explodingTarget;
	Mesh* allyChar, *enemyChar;

	std::vector<GameObject*> m_shipList;
	GameObject::GAMEOBJECT_TYPE allyFlag, enemyFlag;

	// Vars
	int numBullet;
	bool HomingState;
	double asteroidInterval; // time between each asteroid spawn
	double enemyInterval; // time between each enemy fire
	
	double PLAYER_FIRERATE_INTERVAL;
	double EXPLODING_SHIP_SPEED;
};

#endif
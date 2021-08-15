#ifndef SCENE_LEVEL1_H
#define SCENE_LEVEL1_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Button.h"

class SceneLevel1 : public SceneBase
{
	static const int MAX_SPEED = 20; // Max Player Ship Velocity
	static const int BULLET_SPEED = 70; // Player Ship's Bullet Speed
	static const int ENEMY_BULLET_SPEED = 50; // Enemy Ship (Standard&Stationed) Bullet Speed
	static const int MAX_ANGULAR_SPEED = 1; // Max Player Ship Angular Velocity
	static const int STANDARD_ENEMY_SHIP_SPEED = 3; // Standard Enemy Ship Speed
	static const int BUDDY_SHIP_SPEED = 15; // Buddy Ship Speed
	static const int ENEMY_FIRERATE_INTERVAL = 1; // Time Between each enemy ship (Standard&Stationed) fire
	static const int BUDDY_FIRERATE_INTERVAL = 1; // Time Between each buddy ship fire

public:
	SceneLevel1();
	~SceneLevel1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go); // Render GO based on their type
	void RenderButton(Button button); // Render Button
	void UpdateGame(double dt); // Game Related Updates
	void UpdatePlayerShip(double dt); // Player ship Updates
	void UpdateBuddyShip(GameObject* go, double dt); // Buddy ship Updates
	void UpdateBullet(GameObject* go); // Bullet Updates
	void UpdateEnemyBullet(GameObject* go); // Enemy Bullet Updates
	void UpdateAsteroid(GameObject* go); // Asteroid Updates
	void UpdateStandardEnemy(GameObject* go, double dt); // Standard Enemy Ship Updates
	void UpdateStationedEnemy(GameObject* go, double dt); // Stationed Enemy Ship Updates
	void UpdateExplodingEnemy(GameObject* go, double dt); // Exploding Enemy Ship Updates
	void UpdateAttackPowerup(GameObject* go); // Attack Power Up Updates
	void UpdateBuddyPowerup(GameObject* go); // Buddy Power Up Updates
	bool CheckCollision(GameObject* go1, GameObject* go2); // Checks for collision btw two GOs
	bool CheckZero(double number); // Checks if a number is 0 [EPSILON things]
	int GetNumGOs(GameObject::GAMEOBJECT_TYPE type); // Counts number of active GOs of a certian type
	int GetNumGOs(GameObject::GAMEOBJECT_TYPE type[], int arrSize); // Counts number of active GOs of a few types
	Vector3 GetAvailablePos(); // Return position that is not near player
	GameObject* GetNearestGO(Vector3 pos, GameObject::GAMEOBJECT_TYPE type[], int arrSize); // Returns Nearest Active GO of a certain type
	void RestartLevel(); // Resets stats/vars when leaving the level
	bool CheckButtonClick(Button button); // Check for button click

	GameObject* FetchGO(); // Fetch available GO

protected:

	// Game States
	enum GAME_STATE
	{
		G_START,
		G_PLAYING,
		G_PAUSED,
		G_WON,
		G_LOST,
		GS_TOTAL
	};

	// Grid 
	enum GRID
	{
		TOPLEFT,
		TOPRIGHT,
		BOTLEFT,
		BOTRIGHT,

		GRID_TOTAL
	};

	// For messages/warnings to appear on screen
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

	// Mouse Click vars
	bool mousePressed;
	bool mouseClicked;

	// Buttons
	Button startButton;
	Button resumeButton;
	Button menuButton;
	Button nextButton;
	Button restartButton;


	// Physics
	Vector3 m_force;
	Vector3 m_torque;

	// Application
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;


	// Game Related
	int m_lives;
	int m_score;
	std::string level_description;
	std::vector<PopUp*> messageList;

	GAME_STATE gameState;
	std::vector<GameObject *> m_goList;

	// Unique/Single Game Objects
	GameObject* m_ship;
	GameObject* m_buddy;
	GameObject::GAMEOBJECT_TYPE powerupType[2];
	GameObject::GAMEOBJECT_TYPE enemyType[3];

	// Vars
	int numBullet; // num of bullets shot by player ship with each fire
	bool HomingState; // Exploding Enemy Ship State (Homing or not)
	double asteroidInterval; // time between each asteroid spawn
	double enemyInterval; // time between each enemy spawn

	double PLAYER_FIRERATE_INTERVAL; // Time between each player ship fire
	double EXPLODING_SHIP_SPEED; // Speed of Exploding Enemy Ship 
};

#endif
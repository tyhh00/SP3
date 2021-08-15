#ifndef SCENE_LEVEL3_H
#define SCENE_LEVEL3_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Button.h"

class SceneLevel3 : public SceneBase
{
	static const int MAX_SPEED = 20;
	static const int BULLET_SPEED = 70;
	static const int ENEMY_BULLET_SPEED = 50;
	static const int BOSS_MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;
	static const int MAX_ANGULAR_SPEED = 1;
	static const int STANDARD_ENEMY_SHIP_SPEED = 3;
	static const int BUDDY_SHIP_SPEED = 15;
	static const int ENEMY_FIRERATE_INTERVAL = 1;
	static const int BUDDY_FIRERATE_INTERVAL = 1;
	static const int BOSS_FIRERATE_INTERVAL = 1;
	static const int BOSS_STATE_INTERVAL = 7;

public:
	SceneLevel3();
	~SceneLevel3();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	void RenderButton(Button button);
	void UpdateGame(double dt);
	void UpdatePlayerShip(double dt);
	void UpdateBossShip(double dt);
	void UpdateBuddyShip(GameObject* go, double dt);
	void UpdateBullet(GameObject* go);
	void UpdateEnemyBullet(GameObject* go);
	void UpdateBossBullet(GameObject* go, double dt);
	void UpdateBossMissile(GameObject* go, double dt);
	void UpdateAsteroid(GameObject* go);
	void UpdateStandardEnemy(GameObject* go, double dt);
	void UpdateStationedEnemy(GameObject* go, double dt);
	void UpdateExplodingEnemy(GameObject* go, double dt);
	void UpdateAttackPowerup(GameObject* go);
	void UpdateBuddyPowerup(GameObject* go);
	bool CheckCollision(GameObject* go1, GameObject* go2);
	bool CheckZero(double number);
	int GetNumGOs(GameObject::GAMEOBJECT_TYPE type);
	int GetNumGOs(GameObject::GAMEOBJECT_TYPE type[], int arrSize);
	Vector3 GetAvailablePos();
	GameObject* GetNearestGO(Vector3 pos, GameObject::GAMEOBJECT_TYPE type[], int arrSize);
	void RestartLevel();
	bool CheckButtonClick(Button button);

	GameObject* FetchGO();
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

	enum GAME_STATE
	{
		G_START,
		G_PLAYING,
		G_PAUSED,
		G_WON,
		G_LOST,

		GS_TOTAL
	};

	enum BOSS_STATE
	{
		BS_DEFAULT,
		BS_CIRCLE,
		BS_MISSILES,
		BS_SPAWNENEMY,

		BS_TOTAL
	};

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


	bool mousePressed;
	bool mouseClicked;

	Button startButton;
	Button resumeButton;
	Button menuButton;
	//Button nextButton;
	Button restartButton;

	GAME_STATE gameState;

	// Physics
	std::vector<GameObject *> m_goList;
	Vector3 m_force;
	Vector3 m_torque;

	float m_speed;
	float m_worldWidth;
	float m_worldHeight;

	// Game Related
	int m_lives;
	int m_score;
	std::string level_description;
	std::vector<PopUp*> messageList;

	// Unique/Single Game Objects
	GameObject* m_ship;
	GameObject* m_buddy;
	GameObject* m_boss;
	BOSS_STATE bossState;
	// Game Object Type Array
	GameObject::GAMEOBJECT_TYPE powerupType[2];
	GameObject::GAMEOBJECT_TYPE enemyType[4];

	// Vars
	int numBullet;
	bool HomingState;
	double asteroidInterval; // time between each asteroid spawn

	double bossStateInterval;
	double bossMovementInterval;

	double BOSS_MOVEMENT_INTERVAL;
	double PLAYER_FIRERATE_INTERVAL;
	double EXPLODING_SHIP_SPEED;
};

#endif
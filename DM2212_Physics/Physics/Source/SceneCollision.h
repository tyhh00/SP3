//#ifndef SCENE_COLLISION_H
//#define SCENE_COLLISION_H
//
//#include "GameObject.h"
//#include <vector>
//#include "SceneBase.h"
//
//class SceneCollision : public SceneBase
//{
//public:
//	SceneCollision();
//	~SceneCollision();
//
//	virtual void Init();
//	virtual void Update(double dt);
//	virtual void Render();
//	virtual void Exit();
//
//	void RenderGO(GameObject *go);
//
//protected:
//
//	struct Wall {
//		Vector3 N;
//		Vector3 pos;
//		Vector3 scale;
//		Component col;
//
//		Wall(Vector3 pos, float l, float w, Vector3 normal) : col(1, 1, 1)
//		{
//			this->pos = pos;
//			this->scale.Set(w * 0.5, l * 0.5, 1);
//			this->N = normal;
//		}
//
//		Wall(Vector3 pos, float l, float w, int angle) : col(1,1,1)
//		{
//			float theta = Math::DegreeToRadian(angle);
//			this->pos = pos;
//			this->scale.Set(w * 0.5, l * 0.5, 1);
//			this->N.Set(cos(theta), sin(theta), 0);
//		}
//
//		void SetColour(Component col)
//		{
//			this->col = col;
//		}
//	};
//
//	struct Pillar {
//		Vector3 pos;
//		Vector3 scale;
//		Component col;
//
//		Pillar() : col(1, 1, 1)
//		{
//
//		}
//
//		Pillar(Vector3 pos, Vector3 scale) : col(1,1,1)
//		{
//			this->pos = pos;
//			this->scale = scale;
//		}
//	};
//
//	struct Message {
//		Color col;
//		std::string text;
//
//		Message(std::string message, Color col)
//		{
//			text = message;
//			this->col = col;
//		}
//	};
//
//	//Physics
//	std::vector<GameObject *> m_goList;
//	std::vector<Wall*> wallList;
//	std::vector<Pillar*> pillarList;
//	float m_speed;
//	float m_worldWidth;
//	float m_gameWidth;
//	float m_worldHeight;
//
//	int m_score;
//	int m_highscore;
//	int numBalls;
//	Vector3 m_gravity;
//
//	
//	GameObject *m_ghost;
//	int m_objectCount;
//
//	// vars - level layout/design
//	float offset_wall_x, inner_width;
//
//	// vars - flipper, player ball, spring
//	float pBall_radius;
//	Vector3 pBall_ori_pos;
//	float f_radius1, f_radius2;
//	float f_length;
//	float f_max_d;
//	float f_ori_d;
//	float spring_ori_y;
//	float spring_max_d;
//	float sholder_ori_height;
//
//	// GOS
//	GameObject* pBall;
//	GameObject* leftFlipper, *rightFlipper;
//	GameObject* spring, *springHolder;
//	GameObject* topPipe;
//	GameObject* bonus1,* bonus2;
//	GameObject* mWall1[3], *mWall2[3];
//	GameObject* bWall1[3], *bWall2[3];
//	GameObject* target100, *target50, *target10;
//
//	// UI tings
//	std::deque<Message*> messageList;
//
//	GameObject* FetchGO();
//	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);
//	void ReturnGO(GameObject *go);
//
//	void AddPillars(Wall* wall); //Add pillars to wall
//	void AddPillars(GameObject* wall); //Add pillars to wall
//	void AddPillars(GameObject* wall, GameObject** pillar1, GameObject** pillar2); //Add pillars to wall and set ptrs to both pillars
//
//	bool CheckZero(double number); // Checks if a number is 0 [EPSILON things]
//	bool CheckCollision(GameObject* go1, GameObject* go2, float dt); // Checks for collision 
//	void CollisionResponse(GameObject* go1, GameObject* go2, float dt);
//	void Constraint(GameObject* ball, GameObject* other); // Moves ball out of other object upon collision
//	void ApplyFriction(GameObject* ball, Vector3 normal, double dt); // Apply Friction
//	void ApplyBumper(GameObject* ball, Vector3 normal, double dt); //Increase vel of object
//	void ApplyInelastic(GameObject* ball, Vector3 normal, double dt); //Decrease vel of object
//	int GetContactWithFlipper(GameObject* ball, GameObject* flipper); // Check which part of flipper is collided
//
//};
//
//#endif
#pragma once
#include "Application.h"
#include "SingletonTemplate.h"

//Worlds
#include "ScenePhysics.h"
#include "SceneJungle.h"
#include "LevelEditor.h"
#include "SceneRobot.h"
#include "SceneGraveyard.h"
#include "SceneOcean.h"

enum worlds
{
	w_physics = 0,
	w_jungle,
	w_graveyard,
	w_robot,
	w_ocean,
	w_levelEditor,
	w_sceneTotal
};

class SceneManager : public CSingletonTemplate<SceneManager>
{
	friend CSingletonTemplate<SceneManager>;
public:
	int sceneNum; //scene number
	SceneBase* physics, *jungle, * graveyard, * levelEditor, * robot, *church, *ocean; //world pointers
	SceneBase* activeScene; //active scene pointer

	SceneManager(); //constructor
	~SceneManager(); //destructor

	void init(); //initialiser
	void setScene(worlds sceneType); //set scene to scene type chosen
	void resetScene(); //resets scene
	SceneBase* getScene(); //returns scene it currently points to
	void update(double dt); //updates current scene it is pointing to
	void render(); //renders current scene it is pointing to
	void destroy(); //destroys current scene it is pointing to
};

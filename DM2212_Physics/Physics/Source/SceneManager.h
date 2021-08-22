#pragma once
#include "Application.h"
#include "SingletonTemplate.h"

//Worlds
#include "ScenePhysics.h"
#include "SceneJungle.h"
#include "SceneLobby.h"
#include "LevelEditor.h"

#include "SceneRobot.h"
#include "SceneGraveyard.h"
#include "SceneChurch.h"

enum worlds
{
	w_physics = 0,
	w_jungle,
	w_graveyard,
	w_church,
	w_robot,
	w_levelEditor,
	w_lobby,
	w_sceneTotal
};

class SceneManager : public SceneBase, public CSingletonTemplate<SceneManager>
{
	friend CSingletonTemplate<SceneManager>;
public:
	int sceneNum; 
	SceneBase* physics, *jungle, * graveyard, * levelEditor, * lobby, * robot, *church;
	SceneBase* activeScene;

	SceneManager();
	~SceneManager();

	void init();
	void setScene(worlds sceneType);
	SceneBase* getScene();
	void update(double dt);
	void render();
	void destroy();
};

#pragma once
#include "Application.h"
#include "SingletonTemplate.h"

//Worlds
#include "ScenePhysics.h"
#include "SceneTest.h"
#include "SceneGraveyard.h"
#include "SceneLobby.h"
#include "LevelEditor.h"

enum worlds
{
	w_physics = 0,
	w_graveyard,
	w_levelEditor,
	w_lobby,
	w_sceneTotal
};

class SceneManager : public SceneBase, public CSingletonTemplate<SceneManager>
{
	friend CSingletonTemplate<SceneManager>;
public:
	int sceneNum; 
	SceneBase* physics, * graveyard, * levelEditor, * lobby;
	SceneBase* activeScene;

	SceneManager();
	~SceneManager();

	void init();
	void setScene(worlds sceneType);
	void update(double dt);
	void render();
	void destroy();
};

#pragma once
#include "Application.h"
#include "SingletonTemplate.h"

//Worlds
#include "ScenePhysics.h"
#include "SceneTest.h"
#include "SceneLight.h"
#include "LevelEditor.h"

enum worlds
{
	w_physics = 0,
	w_test,
	w_levelEditor,
	w_sceneTotal
};

class SceneManager : public SceneBase, public CSingletonTemplate<SceneManager>
{
	friend CSingletonTemplate<SceneManager>;
public:
	int sceneNum; 
	Scene* physics, * test, * levelEditor;
	Scene* activeScene;

	SceneManager();
	~SceneManager();

	void init();
	void setScene(worlds sceneType);
	void update(double dt);
	void render();
	void destroy();
};

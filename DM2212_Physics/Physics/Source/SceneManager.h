#pragma once
#include "Application.h"
#include "SingletonTemplate.h"

//Worlds
#include "SceneCollision.h"
#include "SceneTest.h"

enum worlds
{
	collision = 0,
	test,
	sceneTotal
};

class SceneManager : public SceneBase, public CSingletonTemplate<SceneManager>
{
	friend CSingletonTemplate<SceneManager>;
public:
	int sceneNum;
	std::vector<Scene*>scenes;
	worlds activeScene;

	SceneManager();
	~SceneManager();

	void init();
	Scene* getScene();
	void destroy();
};

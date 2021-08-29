#include "SceneManager.h"

#include <iostream>

using namespace std;

SceneManager::SceneManager()
	: activeScene(NULL)
{
	physics = new ScenePhysics();
	jungle = new SceneJungle();
	levelEditor = new LevelEditor();
	graveyard = new SceneGraveyard();
	robot = new SceneRobot();
	ocean = new SceneOcean();
}

SceneManager::~SceneManager()
{
	if (physics)
	{
		delete physics;
		physics = NULL;
	}
	if (levelEditor)
	{
		delete levelEditor;
		levelEditor = NULL;
	}
	if (jungle)
	{
		delete jungle;
		jungle = NULL;
	}
	if (graveyard)
	{
		delete graveyard;
		graveyard = NULL;
	}
	if (ocean)
	{
		delete ocean;
		ocean = NULL;
	}
	if (robot)
	{
		delete robot;
		robot = NULL;
	}
}

void SceneManager::init()
{
}

void SceneManager::setScene(worlds sceneType)
{
	switch (sceneType)
	{
	case w_physics:
		activeScene = physics;
		break;
	case w_jungle:
		activeScene = jungle;
		break;
	case w_graveyard:
		activeScene = graveyard;
		break;
	case w_levelEditor:
		activeScene = levelEditor;
		break;
	case w_robot:
		activeScene = robot;
		break;
	case w_ocean:
		activeScene = ocean;
		break;
	}
	activeScene->Init();
	activeScene->InitLights();
	
}
void SceneManager::resetScene()
{
	activeScene->Exit();
	activeScene->Init();
	activeScene->InitLights();
}
SceneBase* SceneManager::getScene()
{
	return activeScene;
}
void SceneManager::update(double dt)
{
	activeScene->Update(dt);

}

void SceneManager::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
	activeScene->Render();
}

void SceneManager::destroy()
{
	activeScene->Exit();
}


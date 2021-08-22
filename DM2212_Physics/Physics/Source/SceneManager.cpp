#include "SceneManager.h"

#include <iostream>

using namespace std;

SceneManager::SceneManager()
	: activeScene(NULL)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::init()
{
	physics = new ScenePhysics();
	physics->Init();
	jungle = new SceneJungle();
	jungle->Init();
	levelEditor = new LevelEditor();
	levelEditor->Init();
	graveyard = new SceneGraveyard();
	graveyard->Init();
	church = new SceneChurch();
	church->Init();
	lobby = new SceneLobby();
	lobby->Init();
	robot = new SceneRobot();
	robot->Init();
	std::cout << "FINISHED INITTING ALL SCENES" << std::endl;
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
	case w_church:
		activeScene = church;
		break;
	case w_levelEditor:
		activeScene = levelEditor;
		break;
	case w_lobby:
		activeScene = lobby;
		break;
	case w_robot:
		activeScene = robot;
		break;
	}
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
	physics->Exit();
	graveyard->Exit();
	levelEditor->Exit();
	lobby->Exit();
	robot->Exit();
}


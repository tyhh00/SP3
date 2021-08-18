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
	levelEditor = new LevelEditor();
	levelEditor->Init();
	graveyard = new SceneGraveyard();
	graveyard->Init();
	lobby = new SceneLobby();
	lobby->Init();
	
}

void SceneManager::setScene(worlds sceneType)
{
	switch (sceneType)
	{
	case w_physics:
		activeScene = physics;
		break;
	case w_graveyard:
		activeScene = graveyard;
		break;
	case w_levelEditor:
		activeScene = levelEditor;
		break;
	case w_lobby:
		activeScene = lobby;
		break;
	}
	activeScene->SetLights();
	
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
}


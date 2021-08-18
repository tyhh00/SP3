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
	test = new SceneTest();
	test->Init();
	
}

void SceneManager::setScene(worlds sceneType)
{
	switch (sceneType)
	{
	case w_physics:
		activeScene = physics;
		break;
	case w_test:
		activeScene = test;
		break;
	case w_levelEditor:
		activeScene = levelEditor;
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
	test->Exit();
	levelEditor->Exit();
}


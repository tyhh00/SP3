#include "SceneManager.h"

#include <iostream>

using namespace std;

SceneManager::SceneManager()
{
	Scene* worldScene = new SceneCollision();
	scenes.push_back(worldScene);
}

SceneManager::~SceneManager()
{
}

void SceneManager::init()
{
	for (int i = 0; i < scenes.size(); i++)
	{
		scenes.at(i)->Init();
	}
	activeScene = collision;
}

Scene* SceneManager::getScene()
{
	return scenes[activeScene];
}

void SceneManager::destroy()
{
	for (int i = 0; i < scenes.size(); i++)
	{
		scenes.at(i)->Exit();
	}
}


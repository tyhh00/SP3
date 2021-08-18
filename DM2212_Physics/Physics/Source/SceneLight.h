#ifndef SCENE_LIGHT_H
#define SCENE_LIGHT_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Keyboard.h"

class SceneLight : public SceneBase
{
public:
	SceneLight();
	~SceneLight();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

protected:


	

	float m_speed;
	float m_screenWidth;
	float m_screenHeight;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	

};

#endif
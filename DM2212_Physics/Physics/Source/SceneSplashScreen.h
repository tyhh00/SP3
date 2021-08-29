#ifndef SCENE_SPLASHSCREEN_H
#define SCENE_SPLASHSCREEN_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Input.h"

class SceneSplashScreen : public SceneBase
{
public:
	SceneSplashScreen();
	~SceneSplashScreen();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

protected:

	enum ANIM_STATE {

		AS_BRIGHTEN,
		AS_WAIT,

		ASTATE_TOTAL
	};

	ANIM_STATE ASTATE;
	double AS_timer;

	float m_speed;

	int m_objectCount;

};

#endif
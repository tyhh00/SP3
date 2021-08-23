#ifndef SCENE_MAINMENU_H
#define SCENE_MAINMENU_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Input.h"
#include "Buttons/ButtonManager.h"

class SceneMainMenu : public SceneBase
{
public:
	SceneMainMenu();
	~SceneMainMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	Mesh* buttonMesh;
	ButtonManager* buttonManager;

};

#endif
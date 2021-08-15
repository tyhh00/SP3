#ifndef SCENE_MAINMENU_H
#define SCENE_MAINMENU_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Button.h"

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

	float m_worldWidth;
	float m_worldHeight;

	bool mousePressed, mouseClicked; // Vars for checking for mouse click

	std::vector<Button*> buttonList; // List of Buttons
	bool CheckButtonClick(Button button); // Check for Button Click
	void RenderButton(Button* button); // Render Button
};

#endif
#ifndef SCENE_LEVELSELECTION_H
#define SCENE_LEVELSELECTION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Button.h"

class SceneLevelSelection : public SceneBase
{
	

public:
	SceneLevelSelection();
	~SceneLevelSelection();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	
protected:

	float m_worldWidth;
	float m_worldHeight;

	bool mousePressed, mouseClicked; // Vars for checking Mouse Click

	std::vector<Button*> buttonList; // List of buttons
	bool CheckButtonClick(Button button); // Check For Button Click
	void RenderButton(Button* button); // Render Button
};

#endif
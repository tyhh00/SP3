#pragma once
#include "Button.h"
#include "Text.h"
#include <vector>

/******************************************************************************/
/*!
		Class ButtonCollide:
\brief The object in vector<ButtonCollide> return from Update() in ButtonManager
*/
/******************************************************************************/
struct ButtonCollide {
	Button* buttonClicked;
	bool justClicked;
	bool isClicking;
	int clickTally; //used to check if already Clicked once, if so set justClicked to false

	//Uses elapsed time of Scene (Hence why ButtonManager is Scene Tied)
	float firstInteractTime;
	float lastClickedTime;

	bool justHovered;
	const bool hovering;
	ButtonCollide(Button* button, float firstInteract) : buttonClicked(button), justClicked(false), isClicking(false), clickTally(0), justHovered(true), hovering(true), firstInteractTime(firstInteract), lastClickedTime(0.f) {}
};

/******************************************************************************/
/*!
\file	ButtonManager.h
\author Tan Yong Hong
\brief
Manages all Buttons in a single instance.

Helps check for Button Hovers, Button Clicks etc.
*/
/******************************************************************************/

/******************************************************************************/
/*!
		Class ButtonManager:
\brief Manages all Buttons in a single instance.
*/
/******************************************************************************/
class ButtonManager
{
	std::vector<Button*> buttons;
	
	//Frequently updated in Update(dt) function:
	std::vector<ButtonCollide*> buttonsInteractedThisTick;

	float screenWidth, screenHeight;
	float elapsed;

public:
	ButtonManager();
	ButtonManager(float screenWidth, float screenHeight);
	~ButtonManager();

	void setScreenSize(float screenWidth, float screenHeight);
	
	//@DEPRECATED
	void Update(SceneBase* scene, double dt);

	void Update(double dt);
	void Render(SceneBase * scene);

	void addButton(Button* button);
	bool deleteButton(Button* button);
	bool activateButton(std::string buttonName);
	bool deactivateButton(std::string buttonName);
	Button* getButtonByName(std::string buttonName);
	std::vector<Button*> getButtons();

	std::vector<ButtonCollide*> getButtonsInteracted();
	void CursorPosition(double& theX, double& theY);

};


#pragma once
#include "../Mesh.h"
#include "../SceneBase.h"

#include "Text.h"
#include "UIItem.h"
#include <string>

/******************************************************************************/
/*!
\file	Button.h
\author Tan Yong Hong
\brief
Buttons creatable for UI.
*/
/******************************************************************************/

/******************************************************************************/
/*!
		Class Button:
\brief Buttons creatable for UI. Used by ButtonManager
*/
/******************************************************************************/
class Button
{
	bool enabled; // whether a button is enabled
	const std::string buttonName; // the button's name
	UIItem UIInfo; // UIInfo of a button storing origin position and size.
	Mesh* quadTexture; // Image of the button (if any)
	//Text Component 
	Text* text; // optional text compoenent of a button.

public:
	Button(std::string buttonName, float originX, float originY, float quadXSize, float quadYSize, Mesh* quadTexture);
	void spawnTextObject(std::string text, Color txtColor, FONTTYPE type, float textSize);
	void disable();
	void enable();
	bool isEnabled();
	std::string getName();
	bool hasText();
	bool isInRange(double, double);
	void setOrigin(float x, float Y);
	UIItem getUIInfo();
	void setTextObject(Text* textPtr);
	void setText(std::string text);
	void setText(std::ostringstream text);
	Text* getTextObject();
	void setQuadImage(Mesh* type);

	void Render(SceneBase* scene);

	~Button();
};


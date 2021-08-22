#pragma once
#include "Button.h"
#include "../Mesh.h"
#include "Text.h"
#include <string>

class ButtonFactory
{
	Button* createTextButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, float offsetX, float offsetY, Color textColour, std::string textString, float textSize);
	Button* createTextButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, float offsetX, float offsetY, Color textColour, std::string textString, float textSize, FONTTYPE type);
	Button* createNoTextButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, Mesh* quadTexture);
	Button* createButton(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, Mesh* quadTexture, float offsetX, float offsetY, Color textColour, std::string textString, float textSize);
};


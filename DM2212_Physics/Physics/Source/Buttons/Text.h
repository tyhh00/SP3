#pragma once
#include <string>
#include "../SceneBase.h"
#include "UIItem.h"

#include <sstream>

/******************************************************************************/
/*!
\file	Text.h
\author Tan Yong Hong
\brief
Text Objects used by Buttons. Used to handle text fonts, text size and associative position
to Buttons.
*/
/******************************************************************************/

enum FONTTYPE {
	CALIBRI,
	SUPERMARIO,
	FONT_COUNT,
};

/******************************************************************************/
/*!
		Class Font:
\brief Text Fonts with thier width attributes and other attributes like vertical offset, which Text Map of Mesh to use from Mesh Handler 
*/
/******************************************************************************/
struct Font {
	FONTTYPE type;
	Mesh* fontMesh;
	int textWidth[256];
	float verticalOffset;

	Font() : type(FONT_COUNT), fontMesh(nullptr) {}
};

/******************************************************************************/
/*!
		Class Text:
\brief Text Objects used by Buttons.
*/
/******************************************************************************/
class Text
{
	const std::string filePath = "TextData//";

	//Static Members
	static Font loadedFonts[FONT_COUNT];
	static void LoadTextData(int textDataArray[], std::string textDataCSV);
	//end Static Members

	Color color;
	UIItem UIInfo;
	Font textFont;
	std::ostringstream text; //accepts \n
	float size;
	bool visible;

	float xOffset, yOffset;

public:
	//Static Public Members
	static bool loadFont(FONTTYPE, std::string widthSheetName, Mesh* fontMesh, float fontVerticalOffset = 1.f);
	static bool areFontsLoaded();
	static Font* getFont(FONTTYPE);
	static void splitText(std::string txt, char delim, std::vector<std::string>& out);
	//end Static Public Members

	Text(Color color, float originX, float originY, float quadSize, FONTTYPE type, float size);
	Text(Color color, UIItem UIInfo, FONTTYPE type, float size);
	~Text();
	void setTextString(std::string text);
	void setTextStringStream(std::ostringstream text);

	//Helps with UI Meshes that have borders, add your own offset here
	void setTextOffsetFromTopLeft(float x, float y);
	void setVisisble(bool visibility);
	std::string getText();
	void updateUIInfo(UIItem info);
	void setColor(Color color);
	Color getColor();
	bool isVisible();
	void setSize(float size);
	void changeFont(FONTTYPE type);
	void Render(SceneBase* scene);

};


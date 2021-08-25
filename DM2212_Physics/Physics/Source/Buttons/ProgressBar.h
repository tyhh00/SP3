#pragma once
#include "Button.h"
/******************************************************************************/
/*!
\file	ProgressBar.h
\author Tan Guan Qun
\brief
Used by Buttons and Text Objects to store their attributes.
*/
/******************************************************************************/

/******************************************************************************/
/*!
		Class ProgressBar:
\brief AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.
*/
/******************************************************************************/

enum PROGRESSBAR_TYPE
{
	HORIZONTAL,
	VERTICAL
};

class ProgressBar : public Button
{
public:
	ProgressBar(std::string buttonName, float originX, float originY, float thickness, float length, PROGRESSBAR_TYPE type, Mesh* quadTexture, float progress = 100);
	~ProgressBar();
	
	void Render(SceneBase* scene);
	void SetProgress(float progress);
	float GetProgress();
private:
	float progress;
	float barWidth;
	float thickness;
	PROGRESSBAR_TYPE barType;
};
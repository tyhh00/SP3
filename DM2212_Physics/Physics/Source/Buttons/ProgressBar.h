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
	ProgressBar(std::string buttonName, float originX, float originY, float width, PROGRESSBAR_TYPE type, Mesh* quadTexture, float progress = 100);
	~ProgressBar();
	
	void Render(SceneBase* scene);
	void SetProgress(float progress);
	float GetProgress();
private:
	float progress;
	float barWidth;
};


//struct ProgressBar {
//	Mesh* mesh;
//
//	float originX, originY;
//	float width, totalLength;
//
//	ProgressBar(Mesh* _mesh, float posX, float posY, float length, float _width) : mesh(_mesh), originX(posX), originY(posY), totalLength(length), width(_width)
//	{
//	}
//
//	void RenderHorizontal(SceneBase* scene, float current, float max)
//	{
//		float scalex = (current / max) * totalLength;
//		float offsetx = (totalLength - scalex) * 0.5;
//		scene->modelStack.PushMatrix();
//		scene->RenderMeshOnScreen(mesh, originX - offsetx, originY, scalex, width);
//		scene->modelStack.PopMatrix();
//	}
//	void RenderVertical(SceneBase* scene, float current, float max)
//	{
//		float scaley = (current / max) * totalLength;
//		float offsety = (totalLength - scaley) * 0.5;
//		scene->modelStack.PushMatrix();
//		scene->RenderMeshOnScreen(mesh, originX, originY - offsety, width, scaley);
//		scene->modelStack.PopMatrix();
//	}
//
//};
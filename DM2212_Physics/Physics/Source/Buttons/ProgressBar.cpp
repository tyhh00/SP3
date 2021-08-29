#include "ProgressBar.h"
#include "MyMath.h"

ProgressBar::ProgressBar(std::string buttonName, float originX, float originY, float thickness, float length, PROGRESSBAR_TYPE type, Mesh* quadTexture, float progress)
	: Button(buttonName, originX, originY, thickness, length, quadTexture)
	, progress(progress)
	, barWidth(length)
	, thickness(thickness)
	, barType(type)
{
	buttonType = PROGRESS_BAR;
}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::SetProgress(float prog)
{
	this->progress = Math::Clamp(prog, 0.f, 1.f);
}

float ProgressBar::GetProgress()
{
	return this->progress;
}

void ProgressBar::Render(SceneBase* scene, int  z)
{
	if (barType == HORIZONTAL)
	{
		float scalex = progress * barWidth;
		float offsetx = (barWidth - scalex) * 0.5;
		scene->modelStack.PushMatrix();
		scene->RenderMeshOnScreen(quadTexture, UIInfo.originX - offsetx, UIInfo.originY, scalex, thickness, 2.0f);
		scene->modelStack.PopMatrix();
	}
	else
	{
		float scaley = progress * barWidth;
		float offsety = (barWidth - scaley) * 0.5;
		scene->modelStack.PushMatrix();
		scene->RenderMeshOnScreen(quadTexture, UIInfo.originX, UIInfo.originY - offsety, thickness, scaley, 2.0f);
		scene->modelStack.PopMatrix();
	}
}


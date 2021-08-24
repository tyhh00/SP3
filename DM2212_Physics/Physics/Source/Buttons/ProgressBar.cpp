#include "ProgressBar.h"
#include "MyMath.h"

ProgressBar::ProgressBar(std::string buttonName, float originX, float originY, float width, PROGRESSBAR_TYPE type, Mesh* quadTexture, float progress)
	: Button(buttonName, originX, originY, width, width, quadTexture)
	, progress(progress)
{

}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::SetProgress(float prog)
{
	this->progress = Math::Clamp(prog, 0.f, 100.f);
}

float ProgressBar::GetProgress()
{
	return this->progress;
}



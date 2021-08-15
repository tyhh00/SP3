
#include "Button.h"

Button::Button(): name(" "), mid(0,0,0), length(1), height(1)
{

}

Button::Button(std::string name, Vector3 pos, float x, float y)
{
	this->name = name;
	this->mid = pos;
	this->length = x;
	this->height = y;
}

void Button::Set(std::string name, Vector3 pos, float x, float y)
{
	this->name = name;
	this->mid = pos;
	this->length = x;
	this->height = y;
}

std::string Button::GetName()
{
	return name;
}

Vector3 Button::GetMid()
{
	return mid;
}

float Button::GetLength()
{
	return length;
}

float Button::GetHeight()
{
	return height;
}

bool Button::CheckWithinButton(int x, int y)
{
	if (x <= mid.x + (length / 2) && x >= mid.x - (length / 2)
		&& y <= mid.y + (height / 2) && y >= mid.y - (height / 2))
	{
		return true;
	}
	return false;
}
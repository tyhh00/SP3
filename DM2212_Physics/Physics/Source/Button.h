#ifndef BUTTON_H
#define BUTTON_H

#include "Vector3.h"
#include <string>

struct Button
{
private:
	std::string name;
	Vector3 mid;
	float length;
	float height;
public:
	Button();
	Button(std::string name, Vector3 pos, float x, float y);
	void Set(std::string name, Vector3 pos, float x, float y);
	std::string GetName();
	Vector3 GetMid();
	float GetLength();
	float GetHeight();
	bool CheckWithinButton(int x, int y);


};

#endif
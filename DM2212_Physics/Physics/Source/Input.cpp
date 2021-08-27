#include "Input.h"
#include "Application.h"
#include <iostream>

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "Debug.h"

extern GLFWwindow* m_window;

Input::Input()
{
	for (int i = 0; i < MAX_KEYBOARD_KEYS; i++)
	{
		prevKeyboardStatus[i] = false;
		currKeyboardStatus[i] = false;
	}
	for (int i = 0; i < MAX_MOUSE_KEYS; i++)
	{
		prevMouseStatus[i] = false;
		currMouseStatus[i] = false;
	}
}

void Input::Update(double dt)
{
	//update all keyboard keys
	for (int i = 0; i < MAX_KEYBOARD_KEYS; i++)
	{
		prevKeyboardStatus[i] = currKeyboardStatus[i];
		if ((GetAsyncKeyState(i) & 0x8001) != 0)
		{
			currKeyboardStatus[i] = true;
		}
		else
		{
			currKeyboardStatus[i] = false;
		}

		//Input Setting (Allowing 0-9, A-Z, _) Using bit 0x0001 for fluid typing
		if (IsKeyPressed(i))
		{
			if(i > 48 && i <= 125)
				typedEntry += i;
		}
		else if ((GetAsyncKeyState(0x00BD) & 0x0001))
			typedEntry += '_';
		else if ((GetAsyncKeyState(VK_BACK) & 0x0001))
		{
			if (!typedEntry.empty())
				typedEntry.pop_back();

		}

	}

	//update all mouse keys
	for (int i = 0; i < MAX_MOUSE_KEYS; i++)
	{
		prevMouseStatus[i] = currMouseStatus[i];
		if (glfwGetMouseButton(m_window, i) != 0)
		{
			currMouseStatus[i] = true;
		}
		else
		{
			currMouseStatus[i] = false;
		}
	}
	
	//DEBUG_MSG(typedEntry);
}

std::string Input::GetTypedEntry()
{
	if (IsKeyReleased(VK_RETURN))
	{
		std::string val = typedEntry;
		typedEntry = "";
		return val;
	}
}

void Input::ClearTypedEntry()
{
	typedEntry = "";
}

bool Input::IsKeyPressed(unsigned short key)
{
	return (!prevKeyboardStatus[key] && currKeyboardStatus[key]);
}

bool Input::IsMousePressed(unsigned short key)
{
	return (!prevMouseStatus[key] && currMouseStatus[key]);
}

bool Input::IsMouseDown(unsigned short key)
{
	return currMouseStatus[key];
}

bool Input::IsMouseReleased(unsigned short key)
{
	return (prevMouseStatus[key] && !currMouseStatus[key]);
}

bool Input::IsKeyDown(unsigned short key)
{
	return currKeyboardStatus[key];
}

bool Input::IsKeyReleased(unsigned short key)
{
	return (prevKeyboardStatus[key] && !currKeyboardStatus[key]);
}

#ifndef INPUT_H
#define INPUT_H
#include "SingletonTemplate.h"

class Input : public CSingletonTemplate<Input> {
	friend CSingletonTemplate<Input>;
public:
	Input();
	void Update(double dt);

	bool IsKeyPressed(unsigned short key);
	bool IsKeyDown(unsigned short key);
	bool IsKeyReleased(unsigned short key);
	
	bool IsMousePressed(unsigned short key);
	bool IsMouseDown(unsigned short key);
	bool IsMouseReleased(unsigned short key);
private:
	static const unsigned short MAX_KEYBOARD_KEYS = 348;
	static const unsigned short MAX_MOUSE_KEYS = 3;
	bool prevKeyboardStatus[MAX_KEYBOARD_KEYS];
	bool currKeyboardStatus[MAX_KEYBOARD_KEYS];
	bool prevMouseStatus[MAX_MOUSE_KEYS];
	bool currMouseStatus[MAX_MOUSE_KEYS];
};


#endif
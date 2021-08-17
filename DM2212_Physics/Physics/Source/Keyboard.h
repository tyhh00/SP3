#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "SingletonTemplate.h"

class Keyboard : public CSingletonTemplate<Keyboard> {
	friend CSingletonTemplate<Keyboard>;
public:
	Keyboard();
	void Init();
	void Update(double dt);
	void PostUpdate(double dt);

	bool IsKeyPressed(unsigned short key);
	bool IsKeyDown(unsigned short key);
	bool IsKeyReleased(unsigned short key);
	
private:
	static const unsigned short MAX_KEYS = 348;
	bool prevStatus[MAX_KEYS];
	bool currStatus[MAX_KEYS];
};


#endif
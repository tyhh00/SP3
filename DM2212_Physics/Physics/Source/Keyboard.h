#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard {
public:
	Keyboard();
	void Init();
	void Update(double dt);

	static bool IsKeyPressed(unsigned short key);
	static bool IsKeyDown(unsigned short key);
	static bool IsKeyReleased(unsigned short key);
	
private:
	static const unsigned short MAX_KEYS = 348;
	static bool prevStatus[MAX_KEYS];
	static bool currStatus[MAX_KEYS];
};


#endif
#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject {
public:
	void Init();
	void Update(double dt);
private:
	float SPEED;
};


#endif
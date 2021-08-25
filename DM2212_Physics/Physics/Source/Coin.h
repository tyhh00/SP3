#pragma once
#include "GameObject.h"
#include "SpriteAnimation.h"
class Coin : public GameObject
{
public:
	Coin(int val);
	~Coin();

	void Init();
	void Update(double dt);
	
	int getCoinVal();

private:
	SpriteAnimation* spriteAnimation;
	
	float yOffset;
	float offsetMax;

	int val;

};


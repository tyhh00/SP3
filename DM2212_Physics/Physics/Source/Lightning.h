#ifndef LIGHTNING_H
#define LIGHTNING_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"

class Lightning : public GameObject {
public:
	Lightning();
	~Lightning();

	void Init(Vector3 pos, Vector3 dir);
	void Update(double dt); /
	void CollidedWith(GameObject* go); //Checks if collided with GOs
	double getTimer(); //returns LightningTimer
	void Render(SceneBase* scene); //Render function

private:
	SpriteAnimation* animatedSprites;
	double lightningTimer; 
};


#endif
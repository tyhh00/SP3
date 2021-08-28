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
	void Update(double dt);
	void CollidedWith(GameObject* go);
	double getTimer();
	void Render(SceneBase* scene);

private:
	SpriteAnimation* animatedSprites;
	double lightningTimer;
};


#endif
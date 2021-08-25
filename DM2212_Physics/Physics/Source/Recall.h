#pragma once
#include "Ability.h"
#include <vector>

#include <queue>

class Recall : public Ability
{
	const float recall_speed = 2.0; //Multiplier of DT so it moves 5s worth of movement in 2.5s
public:
	Recall(GameObject*, double recallTime);
	~Recall();

	void Update(double dt);
	void Render();

private:
	GameObject* assigned;
	double elapsed;

	double recallTime;
	bool recallActive;

	std::queue<std::pair<double, Vector3>> pos_queue;
};


#pragma once
#include "Ability.h"
#include <vector>

#include <queue>

class Recall : public Ability
{
public:
	Recall(GameObject*, double recallTime);
	~Recall();

	void Update(double dt);
	void Render();

private:
	GameObject* assigned;
	double elapsed;
	double recallTime;

	std::queue<std::pair<double, Vector3>> pos_queue;
};


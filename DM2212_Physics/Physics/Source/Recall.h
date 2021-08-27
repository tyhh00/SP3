#pragma once
#include "Ability.h"
#include <vector>


class RecallAbility : public Ability
{
	const float recall_speed = 2.0; //Multiplier of DT so it moves 5s worth of movement in 2.5s
public:
	RecallAbility(GameObject*, double recallTime, Mesh* mesh);
	~RecallAbility();

	void Update(double dt);
	void Render();
	ABILITY_TYPE GetAbilityType();

private:
	GameObject* assigned;
	double elapsed;

	double recallTime;
	bool recallActive;

	double lastActive;

	std::vector<std::pair<double, Vector3>> pos_queue;
};


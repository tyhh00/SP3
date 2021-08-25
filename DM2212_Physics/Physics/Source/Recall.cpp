#include "Recall.h"

Recall::Recall(GameObject* assigned, double recallTime)
	: Ability('Z', ABILITY_RECALL, 10.0)
	, assigned(assigned)
	, recallTime(recallTime)
{}

void Recall::Update(double dt)
{
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0.0)
		abilityCD_timeleft = 0.0;

	//Ability Management
	elapsed += dt;
	if (assigned)
	{
		bool add = true;

		//If Queue at the end
		if (pos_queue.size() > 0 && elapsed - pos_queue.front().first > recallTime)
		{
			pos_queue.pop();
		}
		else
		{
			pos_queue.push(std::pair<double, Vector3>(elapsed, assigned->pos));
		}
	}

	//Activating Ability
	if (abilityCD_timeleft <= 0.0 && !recallActive)
	{
		abilityCD_timeleft = abilityCooldownDuration;
		recallActive = true;
		recallTime = 0.0;
	}

}

void Recall::Render()
{

}
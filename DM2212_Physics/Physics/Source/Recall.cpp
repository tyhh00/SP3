#include "Recall.h"

RecallAbility::RecallAbility(GameObject* assigned, double recallTime)
	: Ability('Z', ABILITY_RECALL, 10.0)
	, assigned(assigned)
	, recallTime(recallTime)
	, recallActive(false)
	, elapsed(0.0)
{}

RecallAbility::~RecallAbility() {}

void RecallAbility::Update(double dt)
{
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0.0)
		abilityCD_timeleft = 0.0;

	//Ability Management
	elapsed += dt;
	if (assigned && !recallActive)
	{
		bool add = true;

		//If Queue at the end
		if (pos_queue.size() > 0 && elapsed - pos_queue.back().first > recallTime)
		{
			pos_queue.erase(pos_queue.begin());
		}
		else
		{
			if( !(pos_queue.size() > 0 && (pos_queue.at(0).second- assigned->pos).LengthSquared() < 0.1 ))
				pos_queue.push_back(std::pair<double, Vector3>(elapsed, assigned->pos));
		}
	}

	//Activating Ability
	if (abilityCD_timeleft <= 0.0 
		&& !recallActive
		&& Input::GetInstance()->IsKeyPressed(buttonChar) 
		&& assigned
		&& elapsed - pos_queue.at(0).first > recallTime-0.3
		)
	{
		abilityCD_timeleft = abilityCooldownDuration;
		recallActive = true;
		assigned->physics->SetMovable(false);
	}

	if (recallActive)
	{
		for (int i = 0; i < recall_speed; ++i)
		{
			if (pos_queue.size() > 0)
			{
				Vector3 pos = pos_queue.back().second;
				assigned->pos = pos;
				assigned->physics->pos = pos;
				pos_queue.pop_back();
			}
			else
			{
				assigned->physics->SetMovable(true);
				recallActive = false;
				pos_queue.clear();
				break;
			}
		}
	}

}

void RecallAbility::Render()
{

}

ABILITY_TYPE RecallAbility::GetAbilityType()
{
	return ABILITY_RECALL;
}
#include "Recall.h"
#include "Debug.h"

#include "SoundController/SoundController.h"

RecallAbility::RecallAbility(double recallTime, Mesh* mesh)
	: Ability('Z', ABILITY_RECALL, 10.0, mesh)
	, recallTime(recallTime)
	, recallActive(false)
	, elapsed(0.0)
	, lastActive(0.0)
{}

RecallAbility::~RecallAbility() {}

void RecallAbility::Init()
{
	assigned = player;
}

void RecallAbility::Update(double dt)
{
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0.0)
		abilityCD_timeleft = 0.0;

	//Ability Management
	elapsed += dt;
	lastActive += dt;
	if (assigned && !recallActive)
	{
		bool add = true;

		//If Queue at the end
		if (pos_queue.size() > 0 && elapsed - pos_queue.at(0).first > recallTime)
		{
			pos_queue.erase(pos_queue.begin());
		}
		if (pos_queue.size() == 0)
		{
			if (lastActive > recallTime)
			{
				pos_queue.push_back(std::pair<double, Vector3>(elapsed, Vector3(assigned->pos)));
			}
		}
		else if (!(pos_queue.at(pos_queue.size() - 1).second - assigned->pos).LengthSquared() < 0.05
			&& lastActive > recallTime)
		{
			pos_queue.push_back(std::pair<double, Vector3>(elapsed, Vector3(assigned->pos)));
		}
		
	}

	//Activating Ability
	if (abilityCD_timeleft <= 0.0 
		&& !recallActive
		&& Input::GetInstance()->IsKeyPressed(buttonChar) 
		&& assigned
		&& lastActive > recallTime
		&& elapsed - pos_queue.at(0).first > recallTime-0.3
		)
	{
		abilityCD_timeleft = abilityCooldownDuration;
		recallActive = true;
		CSoundController::GetInstance()->PlaySoundByID(RECALL_USE);
		assigned->physics->SetMovable(false);
		lastActive = 0.0;
	}

	if (recallActive)
	{
		for (int i = 0; i < recall_speed; ++i)
		{
			if (pos_queue.size() > 0)
			{
				Vector3 pos = pos_queue.at(pos_queue.size()-1).second;
				assigned->pos = pos;
				assigned->physics->pos = pos;
				pos_queue.erase(pos_queue.end()-1);
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

void RecallAbility::Reset()
{
	abilityCD_timeleft = 0;
}

ABILITY_TYPE RecallAbility::GetAbilityType()
{
	return ABILITY_RECALL;
}
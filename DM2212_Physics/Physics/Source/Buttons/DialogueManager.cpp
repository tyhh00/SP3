#include "DialogueManager.h"
#include "../Input.h"
#include "../Application.h"
#include "../MeshBuilder.h"


DialogueManager::DialogueManager() : dialogueBG(NULL)
{
}

DialogueManager::~DialogueManager() {
	for (int i = 0; i < PERSONA_TOTAL; i++)
	{
		if (personaMesh[i])
		{
			delete personaMesh[i];
			personaMesh[i] = NULL;
		}
	}
	if (dialogueBG)
	{
		delete dialogueBG;
	}
}

void DialogueManager::Init(float screenWidth, float screenHeight)
{
	dialogueBG = MeshBuilder::GenerateQuad("dialogue box bg", Color(1, 1, 1), 1.0f);
	dialogueBG->textureID = LoadTGA()
}

void DialogueManager::Update(double dt)
{
	if (dialogueList.size() > 0)
	{
		if (Input::GetInstance()->IsKeyPressed(VK_SPACE))
		{
			Next();
		}
		else
		{
			dialogueList.front()->lifetime -= dt;
			if (dialogueList.front()->lifetime <= 0)
			{
				Next();
			}
		}
	}
}


void DialogueManager::Render(SceneBase* scene)
{
	if (dialogueList.size() > 0)
	{
		// Render .front() Dialogue
	}
}

void DialogueManager::AddDialogue(PERSONA who, std::string message, double lifetime)
{
	dialogueList.push_back(new Dialogue(who, message, lifetime));
}

void DialogueManager::Next()
{
	delete dialogueList.front();
	dialogueList.pop_front();
}



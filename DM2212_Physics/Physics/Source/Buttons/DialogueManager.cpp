#include "DialogueManager.h"
#include "../Input.h"
#include "../Application.h"
#include "../MeshBuilder.h"
#include "../LoadTGA.h"


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

void DialogueManager::Init()
{
	dialogueBG = MeshBuilder::GenerateQuad("dialogue box bg", Color(1, 1, 1), 1.0f);
	dialogueBG->textureID = LoadTGA("Image/DialogueBox.tga");
	personaMesh[PLAYER] = MeshBuilder::GenerateQuad("Player Icon", Color(1, 1, 1), 1.0f);
	personaMesh[PLAYER]->textureID = LoadTGA("Image/Tiles/Player_Girl1.tga");
	personaMesh[GATEKEEPER] = MeshBuilder::GenerateQuad("Player Icon", Color(1, 1, 1), 1.0f);
	personaMesh[GATEKEEPER]->textureID = LoadTGA("Image/Tiles/Gatekeeper.tga");
}

bool DialogueManager::Update(double dt)
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
		return true;
	}
	return false;
}


void DialogueManager::Render(SceneBase* scene)
{
	if (dialogueList.size() > 0)
	{
		// Dialogue Box BG
		scene->RenderMeshOnScreen(dialogueBG, 40, 15, 80, 30);
		
		Dialogue* dialogue = dialogueList.front();
		// Persona Icon
		if (dialogue->displayPos == LEFT)
		{
			scene->RenderMeshOnScreen(dialogue->personaIcon, 20, 19, 10, 10);
		}
		else
		{
			scene->RenderMeshOnScreen(dialogue->personaIcon, 60, 19, 10, 10);
		}
		// Text Dialogue
		scene->RenderTextOnScreen(scene->GetMesh(SceneBase::GEO_TEXT), dialogue->text, Color(1.0f, 1.0f, 1.0f), 2, 20, 30, 50);
	}
}

void DialogueManager::AddDialogue(PERSONA who, std::string message, PERSONA_DISPLAY display_pos, double lifetime)
{
	dialogueList.push_back(new Dialogue(personaMesh[who], message, display_pos, lifetime));
}

void DialogueManager::Next()
{
	delete dialogueList.front();
	dialogueList.pop_front();
}



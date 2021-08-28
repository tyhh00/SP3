#include "DialogueManager.h"
#include "../Input.h"
#include "../Application.h"
#include "../MeshBuilder.h"
#include "../LoadTGA.h"


DialogueManager::DialogueManager() : dialogueBG_left(NULL)
		, dialogueBG_right(NULL)
{
	dialogueBG_left = MeshBuilder::GenerateQuad("dialogue box bg", Color(1, 1, 1), 1.0f);
	dialogueBG_left->textureID = LoadTGA("Image/DialogueBoxRight.tga");
	dialogueBG_right = MeshBuilder::GenerateQuad("dialogue box bg", Color(1, 1, 1), 1.0f);
	dialogueBG_right->textureID = LoadTGA("Image/DialogueBoxLeft.tga");
	personaMesh[PLAYER] = MeshBuilder::GenerateQuad("Player Icon", Color(1, 1, 1), 1.0f);
	personaMesh[PLAYER]->textureID = LoadTGA("Image/PlayerIcon.tga");
	personaMesh[GATEKEEPER] = MeshBuilder::GenerateQuad("Gatekeeper Icon", Color(1, 1, 1), 1.0f);
	personaMesh[GATEKEEPER]->textureID = LoadTGA("Image/GatekeeperIcon.tga");
	personaMesh[GRIMREAPER] = MeshBuilder::GenerateQuad("Grim Reaper Icon", Color(1, 1, 1), 1.0f);
	personaMesh[GRIMREAPER]->textureID = LoadTGA("Image/GrimReaperIcon.tga");
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
	if (dialogueBG_left)
	{
		delete dialogueBG_left;
	}
	if (dialogueBG_right)
	{
		delete dialogueBG_right;
	}
}

void DialogueManager::Init()
{
	
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
		
		Dialogue* dialogue = dialogueList.front();
		// Persona Icon
		if (dialogue->displayPos == LEFT)
		{
			// Dialogue Box BG
			scene->RenderMeshOnScreen(dialogueBG_left, 40, 12, 60, 20);
			scene->RenderMeshOnScreen(dialogue->personaIcon, 10, 15, 20, 30, 0.9f);
		}
		else
		{
			// Dialogue Box BG
			scene->RenderMeshOnScreen(dialogueBG_right, 40, 12, 60, 20);
			scene->RenderMeshOnScreen(dialogue->personaIcon, 70, 15, 20, 30, 0.9f);
		}
		// Text Dialogue
		scene->RenderTextOnScreen(scene->GetMesh(SceneBase::GEO_TEXT), dialogue->text, Color(1.0f, 1.0f, 1.0f), 2, 25, 12, 30, Text::getFont(CALIBRI)->textWidth, 256);
	}
}

void DialogueManager::Exit()
{
	for (int i = 0; i < dialogueList.size(); i++)
	{
		delete dialogueList[i];
	}
	dialogueList.clear();
}

void DialogueManager::AddDialogue(PERSONA who, std::string message, PERSONA_DISPLAY display_pos, double lifetime)
{
	dialogueList.push_back(new Dialogue(personaMesh[who], message, display_pos, lifetime));
}

bool DialogueManager::isDialogue()
{
	if (dialogueList.size() > 0)
	{
		return true;
	}
	return false;
}

void DialogueManager::Next()
{
	delete dialogueList.front();
	dialogueList.pop_front();
}



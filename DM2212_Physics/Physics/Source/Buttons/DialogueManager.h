#pragma once
#include "Button.h"
#include "Text.h"
#include "../Mesh.h"
#include <deque>
#include <string>

enum PERSONA {
	PLAYER,
	GATEKEEPER,
	GRIMREAPER,
	
	PERSONA_TOTAL
};

enum PERSONA_DISPLAY {
	LEFT,
	RIGHT
};

struct Dialogue {
	Mesh* personaIcon;
	PERSONA_DISPLAY displayPos;
	std::string text;
	double lifetime;
	
	Dialogue(Mesh* mesh, std::string message, PERSONA_DISPLAY display_pos = LEFT, double lifetime = 2.0f)
		: personaIcon(mesh), text(message), displayPos(display_pos), lifetime(lifetime)
	{

	}

};

class DialogueManager : public CSingletonTemplate<DialogueManager>
{
	friend CSingletonTemplate<DialogueManager>;
public:
	DialogueManager();
	~DialogueManager();

	void Init();
	bool Update(double dt);
	void Render(SceneBase* scene);
	void Exit();

	void AddDialogue(PERSONA who, std::string message, PERSONA_DISPLAY display_pos = LEFT, double lifetime = 2.0f);
	bool isDialogue();

private:

	Mesh* dialogueBG_left, *dialogueBG_right;
	Mesh* personaMesh[PERSONA_TOTAL];
	std::deque<Dialogue*> dialogueList;

	void Next();

};


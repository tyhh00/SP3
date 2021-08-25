#pragma once
#include "Button.h"
#include "Text.h"
#include "../Mesh.h"
#include <deque>
#include <string>

enum PERSONA {
	PLAYER,
	GATEKEEPER,

	PERSONA_TOTAL
};

struct Dialogue {
	PERSONA persona;
	std::string text;
	double lifetime;
	
	Dialogue(PERSONA who, std::string message, double lifetime = 2.0f)
		: persona(who), text(message), lifetime(lifetime)
	{

	}
};

class DialogueManager : public CSingletonTemplate<DialogueManager>
{
	friend CSingletonTemplate<DialogueManager>;
public:
	DialogueManager();
	~DialogueManager();

	void Init(float screenWidth, float screenHeight);
	void Update(double dt);
	void Render(SceneBase* scene);

	void AddDialogue(PERSONA who, std::string message, double lifetime = 2.0f);

private:
	Mesh* dialogueBG;
	Mesh* personaMesh[PERSONA_TOTAL];
	std::deque<Dialogue*> dialogueList;

	void Next();

};


#pragma once
#include <vector>
#include <string>

#include "GameObject.h"
#include "SceneBase.h"
#include "Debug.h"

//LevelEditor itself will be a scene
//Since we dont need GO_Type, we just use GEO_TYPE(Since its directly working with art)
class LevelEditor : public SceneBase
{

protected:
	std::vector<GameObject*> gridObjects; //Disassociation with GameObjectManager
	int gridLength, gridHeight;


public:
	LevelEditor();
	~LevelEditor();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	bool LoadMap(std::string filename)
	{
		
	}
};


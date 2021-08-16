#pragma once
#include <vector>
#include <string>

#include "GameObject.h"
#include "SceneBase.h"
#include "Debug.h"

//Used in Update Loop of Level Editor, CTRL-Z, Find previous action, and base on type of action, revert changes
enum ACTION_TYPE
{
	CREATE,
	ACTION_DELETE,
	SCALE,
	ROTATE,
	TRANSLATE
};

struct Action
{
protected:
	GameObject* go;
public:
	Action(GameObject* go) : go(go) {}
};

//LevelEditor itself will be a scene
//Since we dont need GO_Type, we just use GEO_TYPE(Since its directly working with art)
class LevelEditor : public SceneBase
{
	const std::string file_path = "Levels\\";

protected:
	std::vector<GameObject*> gridObjects; //Disassociation with GameObjectManager
	int gridLength, gridHeight;
	
	bool mapLoaded;
	std::string mapName;

	bool showGrid;
	bool snapPosToGrid;
	bool snapRotToGrid;
	bool snapScaleToGrid;

	//How to do inventory of tiles

public:
	LevelEditor();
	~LevelEditor();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//Load map from text file
	bool LoadMap(std::string filename);
	
	//Saves map into text file
	void SaveMap();

	//Clears the entire gridObjects list
	void ClearMap();

	//Remove GameObject from gridObjects
	void RemoveGO(GameObject* go);
};


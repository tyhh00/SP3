#pragma once
#include <vector>
#include <string>

#include "GameObject.h"
#include "SceneBase.h"
#include "Debug.h"
#include "LevelLoader.h"
#include "Buttons/ButtonManager.h"
#include <map>
#include <filesystem>

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

struct Pos
{
	double x;
	double y;
	Pos(int x, int y) : x(x), y(y) {}
};

enum SCROLLER_STATE
{
	SCROLLER_TRANSLATE,
	SCROLLER_ROTATE,
	SCROLLER_SCALE,
	SCROLLER_GEOSWITCHER
};

enum RENDERMODE_STATE
{
	RENDER_ALL,
	RENDER_SOLID,
	RENDER_DECORATIVE,
	RENDER_COUNT
};

enum LEVELEDITOR_STATE
{
	MAPSELECTION,
	MAPCREATION,
	LEVELEDIT,
};

//LevelEditor itself will be a scene
//Since we dont need GO_Type, we just use GEO_TYPE(Since its directly working with art)
class LevelEditor : public SceneBase
{
	const float scrollingSpeed = 0.04;
	const float snapRotateSpeed = 0.04;

protected:
	
	ButtonManager bm_le;
	LEVELEDITOR_STATE editorState;
	std::vector<std::string> fileNames;
	
	//Level Editting Variables
	
	std::vector<GameObject*> gridObjects; //Disassociation with GameObjectManager

	std::map<int, std::map<int, std::vector<GameObject*>>> nearbyMap;

	double gridLength, gridHeight;
	
	bool mapLoaded;
	std::string mapName;
	bool unsavedChanges;

	bool showGrid;
	bool snapPosToGrid;
	bool snapRotToGrid;
	bool snapScaleToGrid;

	bool stackOnGrid;

	//bool ctrlToggle;

	SCROLLER_STATE scrollState;

	GameObject* heldOnTo;

	double canScrollIn;
	double canSnapRotateIn;
	GEOMETRY_TYPE scrolledGeo;

	bool decorativeMode;
	RENDERMODE_STATE renderMode;

public:
	LevelEditor();
	~LevelEditor();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//Load map from text file
	bool LoadMap(std::string filename);

	void LoadEditorDefaultValues();
	
	//Saves map into text file
	void SaveMap();

	//Clears the entire gridObjects list
	void ClearMap();

	//Unloads the map from level editor, sending user back to MAPSELECTION
	void UnloadMap();

	//Remove GameObject from gridObjects
	void RemoveGO(GameObject* go);

	std::vector<GameObject*> GetCollidedGOs(double worldPos_X, double worldPos_Y);
	bool PosCollidedWithGO(double worldPos_X, double worldPos_Y, GameObject* go);

	//Utility Functions
	void CursorPosition(double& theX, double& theY);
	void CursorToWorldPosition(double& theX, double& theY);
	bool CursorWithinScreen();
};


#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GameObject.h"
#include <vector>
#include "Button.h"
#include "Shape.h"

typedef struct TileSetting TileSetting;

class SceneBase : public Scene
{
protected:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
public:



	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_TEXT = 1,

		GEO_BALL = 2,
		GEO_CUBE = 3,

		//Tiles Enum Start
		//Make sure to use = to ensure maintenance of IDs (to never conflict)
		GEO_TILES_START = 4,
		GEO_TILEGRID = 5,
		GEO_PLAYER_GIRL1 = 6,
		GEO_BLOCK_UP_RED = 7,

		// GRAVEYARD SCENE TILES
		GEO_GY_CENTER = 8,
		GEO_GY_TOP_MID = 9,
		GEO_GY_TOP_LEFT = 10,
		GEO_GY_TOP_RIGHT = 11,
		GEO_GY_LEFT = 12,
		GEO_GY_RIGHT = 13,
		GEO_GY_BOT_MID = 14,
		GEO_GY_BOT_LEFT = 15,
		GEO_GY_BOT_RIGHT = 16,
		GEO_GY_PLATFORM_LEFT = 17,
		GEO_GY_PLATFORM_RIGHT = 18,
		GEO_GY_PLATFORM_MID = 19,
		GEO_GY_CORNER_LEFT = 20,
		GEO_GY_CORNER_RIGHT = 21,
		GEO_GY_EDGE_LEFT = 22,
		GEO_GY_EDGE_RIGHT = 23,
		GEO_LOBBY_MACHINE1 = 24,
		GEO_LOBBY_MACHINE2 = 25,
		GEO_LOBBY_MACHINE3 = 26,
		GEO_LOBBY_MACHINE4 = 27,
		GEO_LOBBY_MACHINE5 = 28,
		GEO_LOBBY_FLOOR = 29,
		GEO_LOBBY_WALL = 30,
		
		

		//Tiles End
		GEO_TILES_END,


		GEO_BG,

		
		GEO_BUTTON,
		GEO_WALL,

		GEO_100,
		GEO_10,
		GEO_50,
		GEO_HIGHLIGHT,

		GEO_LIGHTBALL,
	
		NUM_GEOMETRY,
	};
public:

	MS modelStack;

	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void SetLights();

	void LoadTile(GEOMETRY_TYPE type, std::string fileName, double length, double height, SHAPE_TYPE shapeType);
	TileSetting* GetTileSetting(GEOMETRY_TYPE type);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, int limit);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
	void RenderGO(GameObject *go);

	GameObject* FetchGO();
protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	TileSetting* tileSize[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera camera;

	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;

	float fps;
};

struct TileSetting
{
	double gridLength, gridHeight;
	SHAPE_TYPE shapeType;
	TileSetting(double length = 1, double height = 1, SHAPE_TYPE shape = SHAPE_TYPE::RECTANGLE) : gridLength(length), gridHeight(height), shapeType(shape) {}
};

#endif
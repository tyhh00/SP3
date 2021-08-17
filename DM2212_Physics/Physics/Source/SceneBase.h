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

		GEO_PLAYER_GIRL1 = 10,

		GEO_BLOCK_UP_RED = 25,

		//Tiles End
		GEO_TILES_END,


		GEO_BG,

		GEO_SHIP,
		GEO_STANDARD_ENEMY,
		GEO_STATIONED_ENEMY,
		GEO_EXPLODING_ENEMY,
		GEO_BOSS,
		GEO_BUDDY,
		GEO_ASTEROID,

		GEO_BULLET,
		GEO_ENEMY_BULLET,
		GEO_BOSS_MISSILE,
		GEO_BOSS_BULLET,

		GEO_ATTACKPOWERUP,
		GEO_BUDDYPOWERUP,

		GEO_BUTTON,
		GEO_EMPTYBAR,
		GEO_BOSSHP,
		GEO_HPBAR,
		GEO_LIVES,
		GEO_TARGET,
		GEO_HOMINGWARNING,
		GEO_TIMEOUT,
		GEO_REDBANNER,
		GEO_BLUEBANNER,

		GEO_CURRY2D,
		GEO_CURRY3D,
		GEO_OMURICE2D,
		GEO_OMURICE3D,
		GEO_YOU,

		GEO_KANAE,
		GEO_MISUMI,


		GEO_UIFRAME,
		GEO_WALL,

		GEO_100,
		GEO_10,
		GEO_50,
		GEO_HIGHLIGHT,

		NUM_GEOMETRY,
	};
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

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

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

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
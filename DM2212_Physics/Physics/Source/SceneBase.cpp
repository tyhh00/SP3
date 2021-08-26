#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "Debug.h"
#include "Input.h"
#include <sstream>
#include "Buttons/Text.h"

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//comg.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(0, 20, 0);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 1;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);


	glUniform1i(m_parameters[U_NUMLIGHTS], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
		tileSize[i] = NULL;
	}

	//@DEPRECATED
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	//Load In Fonts (As of 22.8.21 Update)
	meshList[GEO_FONT_CALIBRI] = MeshBuilder::GenerateText("calibri", 16, 16);
	meshList[GEO_FONT_CALIBRI]->textureID = LoadTGA("Image//calibri.tga");
	Text::loadFont(CALIBRI, "calibri", GEO_FONT_CALIBRI);

	meshList[GEO_FONT_SUPERMARIO] = MeshBuilder::GenerateText("supermario", 16, 16);
	meshList[GEO_FONT_SUPERMARIO]->textureID = LoadTGA("Image//supermario.tga");
	Text::loadFont(SUPERMARIO, "supermario", GEO_FONT_SUPERMARIO);

	//Tiles (Player tile, environmental tiles)
	LoadTile(GEO_TILEGRID, "Grid.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_BLOCK_UP_RED, "Scene2D_GroundTile.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_PLAYER_GIRL1, "Player_Girl1.tga", 1, 1, SHAPE_TYPE::RECTANGLE); //changme y to 1.5
	// graveyard
	LoadTile(GEO_GY_CENTER, "GRAVEYARD_CENTER.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_TOP_MID, "GRAVEYARD_TOPMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_TOP_LEFT, "GRAVEYARD_TOPLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_TOP_RIGHT, "GRAVEYARD_TOPRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_LEFT, "GRAVEYARD_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_RIGHT, "GRAVEYARD_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_BOT_MID, "GRAVEYARD_BOTMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_BOT_LEFT, "GRAVEYARD_BOT_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_BOT_RIGHT, "GRAVEYARD_BOT_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_PLATFORM_LEFT, "GRAVEYARD_PLATFORM_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE); // DONE
	LoadTile(GEO_GY_PLATFORM_RIGHT, "GRAVEYARD_PLATFORM_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_PLATFORM_MID, "GRAVEYARD_PLATFORM_MID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_CORNER_LEFT, "GRAVEYARD_LEFTCORNER.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_CORNER_RIGHT, "GRAVEYARD_RIGHTCORNER.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_EDGE_LEFT, "GRAVEYARD_LEFTEDGE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_EDGE_RIGHT, "GRAVEYARD_RIGHTEDGE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_LOBBY_MACHINE1, "LOBBY_MACHINE_1.tga", 3, 3, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_LOBBY_MACHINE2, "LOBBY_MACHINE_2.tga", 3, 3, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_LOBBY_MACHINE3, "LOBBY_MACHINE_3.tga", 3, 3, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_LOBBY_MACHINE4, "LOBBY_MACHINE_4.tga", 3, 3, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_LOBBY_MACHINE5, "LOBBY_MACHINE_5.tga", 3, 3, SHAPE_TYPE::RECTANGLE);
	
	
	LoadTile(GEO_LOBBY_FLOOR, "LOBBY_FLOOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	//LoadTile(GEO_LOBBY_FLOOR, "Invisible.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_LOBBY_WALL, "LOBBY_WALL.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	
	LoadTile(GEO_OCEAN_1_TOPLEFT, "OCEAN_1_topLeft.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_1_TOPMID, "OCEAN_1_topMid.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_1_TOPRIGHT, "OCEAN_1_topRight.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_1_MIDLEFT, "OCEAN_1_midLeft.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_1_MIDMID, "OCEAN_1_midMid.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_1_MIDRIGHT, "OCEAN_1_midRight.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_1_BOTLEFT, "OCEAN_1_botLeft.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_1_BOTMID, "OCEAN_1_botMid.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_1_BOTRIGHT, "OCEAN_1_botRight.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_2_LEFT, "OCEAN_2_left.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_2_MID, "OCEAN_2_mid.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_2_RIGHT, "OCEAN_2_right.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_3_BLOCK, "OCEAN_3_block.tga", 1, 1, SHAPE_TYPE::RECTANGLE);

	//Robots Theme
	LoadTile(GEO_ROBOT_BIGCUBE_1_TOPLEFT, "ROBOT_BIGCUBE_1_TOPLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_1_TOPMID, "ROBOT_BIGCUBE_1_TOPMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_1_TOPRIGHT, "ROBOT_BIGCUBE_1_TOPRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_1_MIDLEFT, "ROBOT_BIGCUBE_1_MIDLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_1_MIDMID, "ROBOT_BIGCUBE_1_MIDMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_1_MIDRIGHT, "ROBOT_BIGCUBE_1_MIDRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_1_BOTLEFT, "ROBOT_BIGCUBE_1_BOTLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_1_BOTMID, "ROBOT_BIGCUBE_1_BOTMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_1_BOTRIGHT, "ROBOT_BIGCUBE_1_BOTRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_MEDIUMCUBE_2_TOPLEFT, "ROBOT_MEDIUMCUBE_2_TOPLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_MEDIUMCUBE_2_TOPRIGHT, "ROBOT_MEDIUMCUBE_2_TOPRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_MEDIUMCUBE_2_BOTLEFT, "ROBOT_MEDIUMCUBE_2_BOTLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_MEDIUMCUBE_2_BOTRIGHT, "ROBOT_MEDIUMCUBE_2_BOTRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_3_TOPLEFT, "ROBOT_BIGCUBE_3_TOPLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_3_TOPMID, "ROBOT_BIGCUBE_3_TOPMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_3_TOPRIGHT, "ROBOT_BIGCUBE_3_TOPRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_3_MIDLEFT, "ROBOT_BIGCUBE_3_MIDLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_3_MIDMID, "ROBOT_BIGCUBE_3_MIDMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_3_MIDRIGHT, "ROBOT_BIGCUBE_3_MIDRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_3_BOTLEFT, "ROBOT_BIGCUBE_3_BOTLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_3_BOTMID, "ROBOT_BIGCUBE_3_BOTMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_3_BOTRIGHT, "ROBOT_BIGCUBE_3_BOTRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_4_TOPLEFT, "ROBOT_BIGCUBE_4_TOPLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_4_TOPMID, "ROBOT_BIGCUBE_4_TOPMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_4_TOPRIGHT, "ROBOT_BIGCUBE_4_TOPRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_4_MIDLEFT, "ROBOT_BIGCUBE_4_MIDLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_4_MIDMID, "ROBOT_BIGCUBE_4_MIDMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_4_MIDRIGHT, "ROBOT_BIGCUBE_4_MIDRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_4_BOTLEFT, "ROBOT_BIGCUBE_4_BOTLEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_4_BOTMID, "ROBOT_BIGCUBE_4_BOTMID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_BIGCUBE_4_BOTRIGHT, "ROBOT_BIGCUBE_4_BOTRIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_5_TOP, "ROBOT_SMALLCUBE_5_TOP.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_5_BOTTOM, "ROBOT_SMALLCUBE_5_BOTTOM.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_6_TOP, "ROBOT_SMALLCUBE_6_TOP.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_6_MID, "ROBOT_SMALLCUBE_6_MID.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_6_BOTTOM, "ROBOT_SMALLCUBE_6_BOTTOM.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_7_MISCDECOR, "ROBOT_SMALLCUBE_7_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_8_MISCDECOR, "ROBOT_SMALLCUBE_8_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_9_MISCDECOR, "ROBOT_SMALLCUBE_9_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_10_MISCDECOR, "ROBOT_SMALLCUBE_10_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_11_MISCDECOR, "ROBOT_SMALLCUBE_11_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_12_MISCDECOR, "ROBOT_SMALLCUBE_12_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_13_MISCDECOR, "ROBOT_SMALLCUBE_13_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_14_MISCDECOR, "ROBOT_SMALLCUBE_14_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_15_MISCDECOR, "ROBOT_SMALLCUBE_15_MISCDECOR .tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_16_POTION1, "ROBOT_SMALLCUBE_16_POTION1.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_16_POTION2, "ROBOT_SMALLCUBE_16_POTION2 .tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_16_POTION3, "ROBOT_SMALLCUBE_16_POTION3.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_17_DIAMOND1, "ROBOT_SMALLCUBE_17_DIAMOND1.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_17_DIAMOND2, "ROBOT_SMALLCUBE_17_DIAMOND2.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_17_DIAMOND3, "ROBOT_SMALLCUBE_17_DIAMOND3.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_17_DIAMOND4, "ROBOT_SMALLCUBE_17_DIAMOND4.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_18_SIGN1, "ROBOT_SMALLCUBE_18_SIGN1.tga", 1, 1, SHAPE_TYPE::RECTANGLE);


	LoadTile(GEO_ROBOT_SMALLCUBE_6_BOTTOM, "ROBOT_SMALLCUBE_6_BOTTOM.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_7_MISCDECOR, "ROBOT_SMALLCUBE_7_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_8_MISCDECOR, "ROBOT_SMALLCUBE_8_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_9_MISCDECOR, "ROBOT_SMALLCUBE_9_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_10_MISCDECOR, "ROBOT_SMALLCUBE_10_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_11_MISCDECOR, "ROBOT_SMALLCUBE_11_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_12_MISCDECOR, "ROBOT_SMALLCUBE_12_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_13_MISCDECOR, "ROBOT_SMALLCUBE_13_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_14_MISCDECOR, "ROBOT_SMALLCUBE_14_MISCDECOR.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_15_MISCDECOR, "ROBOT_SMALLCUBE_15_MISCDECOR .tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_16_POTION1, "ROBOT_SMALLCUBE_16_POTION1.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_16_POTION2, "ROBOT_SMALLCUBE_16_POTION2 .tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_16_POTION3, "ROBOT_SMALLCUBE_16_POTION3.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_17_DIAMOND1, "ROBOT_SMALLCUBE_17_DIAMOND1.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_17_DIAMOND2, "ROBOT_SMALLCUBE_17_DIAMOND2.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_17_DIAMOND3, "ROBOT_SMALLCUBE_17_DIAMOND3.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_17_DIAMOND4, "ROBOT_SMALLCUBE_17_DIAMOND4.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ROBOT_SMALLCUBE_18_SIGN1, "ROBOT_SMALLCUBE_18_SIGN1.tga", 1, 1, SHAPE_TYPE::RECTANGLE);

	LoadTile(GEO_JUNGLE_DIRT_BLOCK,	"JUNGLE_SNOWMAN.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_CENTER, "JUNGLE_DIRT_CENTER.tga", 1, 1, SHAPE_TYPE::RECTANGLE);//
	LoadTile(GEO_JUNGLE_DIRT_INTERSECTION_BOTTOM_LEFT, "JUNGLE_DIRT_INTERSECTION_BOTTOM_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_INTERSECTION_BOTTOM_RIGHT, "JUNGLE_DIRT_INTERSECTION_BOTTOM_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_INTERSECTION_TOP_LEFT, "JUNGLE_DIRT_INTERSECTION_TOP_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_INTERSECTION_TOP_RIGHT, "JUNGLE_DIRT_INTERSECTION_TOP_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_MIDDLE, "JUNGLE_DIRT_MIDDLE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_MIDDLE_LEFT, "JUNGLE_DIRT_MIDDLE_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_MIDDLE_RIGHT, "JUNGLE_DIRT_MIDDLE_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_MIDDLE_TOP, "JUNGLE_DIRT_MIDDLE_TOP.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_PLATFORM_LEFT, "JUNGLE_DIRT_PLATFORM_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_PLATFORM_MIDDLE, "JUNGLE_DIRT_PLATFORM_MIDDLE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);//
	LoadTile(GEO_JUNGLE_DIRT_PLATFORM_RIGHT, "JUNGLE_DIRT_PLATFORM_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_TOP, "JUNGLE_DIRT_TOP.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_TOP_LEFT, "JUNGLE_DIRT_TOP_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_DIRT_TOP_RIGHT, "JUNGLE_DIRT_TOP_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_GRASS_BLOCK, "JUNGLE_GRASS_BLOCK.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_GRASS_MIDDLE, "JUNGLE_GRASS_MIDDLE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_GRASS_PLATFORM_LEFT, "JUNGLE_GRASS_PLATFORM_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_GRASS_PLATFORM_MIDDLE, "JUNGLE_GRASS_PLATFORM_MIDDLE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_GRASS_PLATFORM_RIGHT, "JUNGLE_GRASS_PLATFORM_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_GRASS_TOP, "JUNGLE_GRASS_TOP.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_GRASS_TOP_LEFT, "JUNGLE_GRASS_TOP_LEFT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_GRASS_TOP_RIGHT, "JUNGLE_GRASS_TOP_RIGHT.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_MONKEY, "GEO_JUNGLE_MONKEY.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_CAMPFIRE, "GEO_JUNGLE_CAMPFIRE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_FIRETORCH, "GEO_JUNGLE_FIRETORCH.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_APPLE, "GEO_JUNGLE_APPLE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_JUNGLE_BANANA, "GEO_JUNGLE_BANANA.tga", 1, 1, SHAPE_TYPE::RECTANGLE);

	LoadTile(GEO_FLASHLIGHT, "flashlight.tga", 0.386, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_BATTERY, "battery.tga", 0.318, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_PICKAXE, "pickaxe.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_BONES_02, "BONES_02.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_BONES_03, "BONES_03.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_BONES_10, "BONES_10.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_BONES_11, "BONES_11.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GRAVEYARD_BUSH1, "GY_BUSH1.tga", 1, 0.495, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GRAVEYARD_BUSH2, "GY_BUSH2.tga", 1, 0.64, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GRAVEYARD_DEADBUSH, "GY_DEADBUSH.tga", 1, 0.56, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GRAVEYARD_TREE, "GY_TREE.tga", 2.86, 2.39, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GRAVEYARD_SIGN, "GY_SIGN.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_CRATE, "CRATE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_TOMBSTONE, "Tombstone.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_TOMBSTONE_CROSS, "CrossTombstonE.tga", 0.697, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GRAVEYARD_ARROWSIGN, "GY_ARROWSIGN.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ENEMY_GHOST, "ghost.tga", 1.9, 2.2, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ENEMY_TUMBLEWEED, "tumbleweed.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_GATEKEEPER, "Gatekeeper.tga", 0.714, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_GY_CHURCH, "church.tga", 2.37, 1.98, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_CHURCH_CENTER, "CHURCH_CENTER.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_CHURCH_PLATFORM_MID, "CHURCH_PLATFORM_CENTER.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_CHURCH_PLATFORM_EDGE, "CHURCH_PLATFORM_EDGE.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_CHURCH_PLATFORM_CORNER, "CHURCH_PLATFORM_CORNER.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_ENEMY_GRIMREAPER, "GrimReaper.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	
	LoadTile(GEO_LOBBY_PORTAL_JUNGLE, "LOBBY_portalFrame.tga", 2, 2, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_LOBBY_PORTAL_GRAVEYARD, "LOBBY_portalFrame.tga", 2, 2, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_LOBBY_PORTAL_OCEAN, "LOBBY_portalFrame.tga", 2, 2, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_LOBBY_PORTAL_ROBOT, "LOBBY_portalFrame.tga", 2, 2, SHAPE_TYPE::RECTANGLE);

	LoadTile(GEO_ROBOT_ENEMY_1, "ROBOT_ENEMY_1.tga", 1, 1, SHAPE_TYPE::RECTANGLE);

	LoadTile(GEO_OCEAN_CRAB, "enemy_crab.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	LoadTile(GEO_OCEAN_DRAGONHEAD, "enemy_dragonHead.tga", 1, 1, SHAPE_TYPE::RECTANGLE);

	LoadTile(GEO_COIN, "coin_tile.tga", 1, 1, SHAPE_TYPE::RECTANGLE);
	
	//Entities (Player, etc)

	meshList[GEO_BLACKHOLE] = MeshBuilder::GenerateQuad("black_hole", Color(1, 1, 1), 2.0f);
	meshList[GEO_BLACKHOLE]->textureID = LoadTGA("Image//black_hole.tga");

	meshList[GEO_HEALTHBAR_OUTER] = MeshBuilder::GenerateQuad("healthbar_outer", Color(1, 1, 1), 5.0f);
	meshList[GEO_HEALTHBAR_OUTER]->textureID = LoadTGA("Image//HealthBar.tga");

	meshList[GEO_HEALTHBAR] = MeshBuilder::GenerateQuad("healthbar", Color(1, 1, 1), 5.0f);
	meshList[GEO_HEALTHBAR]->textureID = LoadTGA("Image//HealthBarInner.tga");

	//Shapes
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_BALL] = MeshBuilder::GenerateSphere("ball", Color(1, 1, 1), 10, 10, 1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 2.f);
	meshList[GEO_BUTTON] = MeshBuilder::GenerateQuad("button", Color(1, 1, 1), 1.0f);
	meshList[GEO_BUTTON]->textureID = LoadTGA("Image/button.tga");

	//UI
	meshList[GEO_UI_APPLE] = MeshBuilder::GenerateQuad("button", Color(1, 1, 1), 1.0f);
	meshList[GEO_UI_APPLE]->textureID = LoadTGA("Image/button.tga");
	meshList[GEO_UI_CHEESE] = MeshBuilder::GenerateQuad("button", Color(1, 1, 1), 1.0f);
	meshList[GEO_UI_CHEESE]->textureID = LoadTGA("Image/button.tga");

	meshList[GEO_WALL] = MeshBuilder::GenerateQuad("Wall", Color(1, 1, 1), 2.0f);
	meshList[GEO_BALL] = MeshBuilder::GenerateCircle("circle", 1.0f, Color(1, 1, 1));
	meshList[GEO_100] = MeshBuilder::GenerateCircle("score tings", 1.0f, Color(1, 1, 1));
	meshList[GEO_100]->textureID = LoadTGA("Image/100.tga");
	meshList[GEO_10] = MeshBuilder::GenerateCircle("score tings", 1.0f, Color(1, 1, 1));
	meshList[GEO_10]->textureID = LoadTGA("Image/10.tga");
	meshList[GEO_50] = MeshBuilder::GenerateCircle("score tings", 1.0f, Color(1, 1, 1));
	meshList[GEO_50]->textureID = LoadTGA("Image/50.tga");
	meshList[GEO_HIGHLIGHT] = MeshBuilder::GenerateCircle("highlight", 1.0f, Color(0.0f, 1.0f, 0.0f));

	bLightEnabled = false;

	gameWin = false;
	gameLost = false;
}

void SceneBase::LoadTile(GEOMETRY_TYPE type, std::string fileName, double length, double height, SHAPE_TYPE shapeType)
{
	std::string path = "Image//Tiles//" + fileName;
	if (type > GEO_TILES_START && type < GEO_TILES_END)
	{
		//Generate cube mesh
		switch (shapeType)
		{
		case RECTANGLE:
			meshList[type] = MeshBuilder::GenerateQuad("Tile-" + fileName, Color(1, 1, 1), 2.f);
			meshList[type]->textureID = LoadTGA(path.c_str());
			break;
		case CIRCLE:
			meshList[type] = MeshBuilder::GenerateCircle("Tile-" + fileName, 1.f, Color(1,1,1));
			meshList[type]->textureID = LoadTGA(path.c_str());
			break;
		}

		//Set TileSize to understand how big the tile is
		if (tileSize[type]) delete tileSize[type];
		tileSize[type] = new TileSetting(length, height, shapeType);
	}
	else
		DEBUG_MSG("Unable to load tile of path " << path << ". With GEOMETRY_TYPE ID: " << type << ".");
	
}

TileSetting* SceneBase::GetTileSetting(GEOMETRY_TYPE type)
{
	if (type > GEO_TILES_START && type < GEO_TILES_END)
		return tileSize[type];
	else
		DEBUG_MSG("Unable to get tile setting of GEOMETRY_TYPE ID: " << type << ". Are you sure this is even a tile?");
}

Mesh* SceneBase::GetMesh(GEOMETRY_TYPE type)
{
	return meshList[type];
}

float SceneBase::GetScreenWidth()
{
	return m_screenWidth;
}

float SceneBase::GetScreenHeight()
{
	return m_screenHeight;
}

float SceneBase::GetWorldWidth()
{
	return m_worldWidth;
}

float SceneBase::GetWorldHeight()
{
	return m_worldHeight;
}

void SceneBase::Update(double dt)
{
	//Input::GetInstance()->Update(dt);

	////Keyboard Section
	//if(Application::IsKeyPressed('1'))
	//	glEnable(GL_CULL_FACE);
	//if(Application::IsKeyPressed('2'))
	//	glDisable(GL_CULL_FACE);
	//if(Application::IsKeyPressed('3'))
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//if(Application::IsKeyPressed('4'))
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	fps = (float)(1.f / dt);
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.6f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 178, 0, 100, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, int limit)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	//Change this line inside for loop
	std::string toPrint;
	if (signed(text.length()) > limit)
	{
		toPrint = text.substr(0, limit);
		for (unsigned i = 0; i < toPrint.length(); ++i)
		{
			Mtx44 characterSpacing;
			characterSpacing.SetToTranslation(0.5f + i * 0.5f, 0.5f, 0);
			Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

			mesh->Render((unsigned)text[i] * 6, 6);
		}
		RenderTextOnScreen(meshList[GEO_TEXT], text.substr(limit, text.length()), color, size, x, y - (1 * size), limit);
	}
	else
	{
		toPrint = text;
		for (unsigned i = 0; i < toPrint.length(); ++i)
		{
			Mtx44 characterSpacing;
			characterSpacing.SetToTranslation(0.5f + i * 0.5f, 0.5f, 0);
			Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

			mesh->Render((unsigned)text[i] * 6, 6);
		}
	}


	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, int(&charWidth)[256], int arrSize)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	//glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 1);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float accumulator = 0;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + accumulator, 0.5f, 1);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);

		accumulator += charWidth[text[i]] / 64.0f;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);


	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneBase::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, float z)
{
	//glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, z);
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneBase::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
		if (tileSize[i])
			delete tileSize[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void SceneBase::InitLights()
{
	bLightEnabled = false;
}

void SceneBase::ToggleLightOnOff(int index, bool on)
{
	if (on)
	{
		lights[index].power = lights[index].defaultPower;
	}
	else
	{
		lights[index].power = 0;
	}
	
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	
}

void SceneBase::ToggleLightPower(int index, int power)
{
	
	lights[index].power = power;
	

	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
}

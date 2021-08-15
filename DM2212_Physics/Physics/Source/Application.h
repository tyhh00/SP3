#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "Scene.h"

class Application
{
	
public:
	enum SCENE_TYPE
	{
		S_MAINMENU,
		S_LEVELSELECT,
		S_LEVEL1,
		S_LEVEL2,
		S_LEVEL3,
		S_COLLISION,
		

		SCENE_TOTAL
	};
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	
	static bool quit;

	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static void GetCursorPos(double *xpos, double *ypos);
	static void SwitchScene(SCENE_TYPE type);
	static int GetWindowWidth();
	static int GetWindowHeight();

private:
	Scene* sceneArray[SCENE_TOTAL];
	static int index;

	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;
};

#endif
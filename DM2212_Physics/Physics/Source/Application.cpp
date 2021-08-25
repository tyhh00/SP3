
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

//Gamestate manager
#include "GameStateManagement/GameStateManager.h"
#include "GameStateManagement/IntroState.h"
#include "GameStateManagement/MenuState.h"
#include "GameStateManagement/LobbyState.h"
#include "GameStateManagement/PlayGameState.h"

#include "GameManager.h"

bool Application::quit = false;

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
int m_width, m_height;
int Application::mouseScrollingUp = 0;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
		Application::mouseScrollingUp = 1; //Scroll Up
	else if (yoffset < 0)
		Application::mouseScrollingUp = 2; //Scroll Down
}


void resize_callback(GLFWwindow* window, int w, int h)
{
	m_width = w;
	m_height = h;
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}
bool Application::IsKeyReleased(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x0001) != 0);
}
bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}
void Application::GetCursorPos(double *xpos, double *ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}

int Application::GetWindowWidth()
{
	return m_width;
}
int Application::GetWindowHeight()
{
	return m_height;
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_width = 1600;
	m_height = 900;
//	m_window = glfwCreateWindow(m_width, m_height, "Physics", glfwGetPrimaryMonitor(), NULL);
	m_window = glfwCreateWindow(m_width, m_height, "Physics", NULL, NULL);
	
	UIManager::GetInstance()->Init();

	GameManager::GetInstance()->Init();

	//create gamestates
	CGameStateManager::GetInstance()->AddGameState("IntroState", new CIntroState());
	CGameStateManager::GetInstance()->AddGameState("MenuState", new CMenuState());
	CGameStateManager::GetInstance()->AddGameState("LobbyState", new CLobbyState());
	CGameStateManager::GetInstance()->AddGameState("PlayGameState", new CPlayGameState());
	

	//set active scene
	CGameStateManager::GetInstance()->SetActiveGameState("IntroState");

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}


}


void Application::Run()
{
	//Main Loop
	//sceneArray[S_COLLISION] = new SceneCollision();
	//sceneArray[S_TEST] = new SceneTest();
	
	//double dElapsedTime = 0.0;

	/*for (int i = 0; i < SCENE_TOTAL; i++)
	{
		sceneArray[i]->Init();
	}*/
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !quit)
	{
		/*if (dElapsedTime > 0.0166666666666667)
			dElapsedTime = 0.0166666666666667;*/

		//update gamestate manager
		
		if (CGameStateManager::GetInstance()->Update(m_timer.getElapsedTime()) == false)
		{
			break;
		}
		//call active gamestate render method
		CGameStateManager::GetInstance()->Render();

		//sceneArray[index]->Update(m_timer.getElapsedTime());
		//sceneArray[index]->Render();
		//Swap buffers
		Application::mouseScrollingUp = 0; //Reset Scrolling

		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed

	//delete everyth
	/*for (int i = 0; i < SCENE_TOTAL; i++)
	{
		sceneArray[i]->Exit();
		delete sceneArray[i];
	}*/
}

void Application::Exit()
{
	//Destroy CGameStateManager
	CGameStateManager::GetInstance()->Destroy();

	SceneManager::GetInstance()->destroy();
	SceneManager::GetInstance()->Destroy();

	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

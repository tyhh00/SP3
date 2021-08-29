#include "SceneLobby.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include <sstream>
#include "LevelLoader.h"
#include "../Source/SoundController/SoundController.h"
#include "Utility.h"
#include <sstream>
#include "Debug.h"

//Entity Includes
#include "Player.h"

//...

SceneLobby::SceneLobby()
{
	dialogueManager = DialogueManager::GetInstance();
	//Store keyboard instance
	input = Input::GetInstance();
	goManager = new GameObjectManager();
	inventory = new Inventory();
	buttonManager = new ButtonManager(80, 60);

	machinePartsUIBG = MeshBuilder::GenerateQuad("Machine Parts Display BG", Color(1, 1, 1), 1.0f);
	machinePartsUIBG->textureID = LoadTGA("Image/TimeMachineBG.tga");
	settingsBG = MeshBuilder::GenerateQuad("Settings BG", Color(1, 1, 1), 1.0f);
	settingsBG->textureID = LoadTGA("Image/TimeMachineBG.tga");
	machinePartsSlot = MeshBuilder::GenerateQuad("Machine Parts Slots", Color(1, 1, 1), 1.0f);
	machinePartsSlot->textureID = LoadTGA("Image/TimeMachinePartsSlot.tga");

	abilityUIBG = MeshBuilder::GenerateQuad("Machine Parts Slots", Color(1, 1, 1), 1.0f);
	abilityUIBG->textureID = LoadTGA("Image/TimeMachineBG.tga");
	grapplingAbilityUI = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	grapplingAbilityUI->textureID = LoadTGA("Image/Grappling.tga");
	dashAbilityUI = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	dashAbilityUI->textureID = LoadTGA("Image/Dash.tga");
	portalAbilityUI = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	portalAbilityUI->textureID = LoadTGA("Image/PortalAbilityIcon.tga");
	recallAbilityUI = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	recallAbilityUI->textureID = LoadTGA("Image/recall_ability.tga");
	slowTimeAbilityUI = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	slowTimeAbilityUI->textureID = LoadTGA("Image/slow_time.tga");
	blackHoleAbilityUI = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	blackHoleAbilityUI->textureID = LoadTGA("Image/black_hole.tga");
	ability1 = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	ability1->textureID = LoadTGA("Image/blackUI.tga");
	ability2 = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	ability2->textureID = LoadTGA("Image/blackUI.tga");
	clearAbility = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	clearAbility->textureID = LoadTGA("Image/Delete.tga");
}

SceneLobby::~SceneLobby()
{
	input = NULL;
	if (goManager)
	{
		delete goManager;
		goManager = NULL;
	}
	if (inventory)
	{
		delete inventory;
		inventory = NULL;
	}
	if (buttonManager)
	{
		delete buttonManager;
		buttonManager = NULL;
	}
	if (machinePartsUIBG)
	{
		delete machinePartsUIBG;
		machinePartsUIBG = NULL;
	}
	if (machinePartsSlot)
	{
		delete machinePartsSlot;
		machinePartsSlot = NULL;
	}
	if (abilityUIBG)
	{
		delete abilityUIBG;
		abilityUIBG = NULL;
	}
	if (dashAbilityUI)
	{
		delete dashAbilityUI;
		dashAbilityUI = NULL;
	}
	if (portalAbilityUI)
	{
		delete portalAbilityUI;
		portalAbilityUI = NULL;
	}
	if (recallAbilityUI)
	{
		delete recallAbilityUI;
		recallAbilityUI = NULL;
	}
	if (slowTimeAbilityUI)
	{
		delete slowTimeAbilityUI;
		slowTimeAbilityUI = NULL;
	}
	if (blackHoleAbilityUI)
	{
		delete blackHoleAbilityUI;
		blackHoleAbilityUI = NULL;
	}	
	if (ability1)
	{
		delete ability1;
		ability1 = NULL;
	}
	if (ability2)
	{
		delete ability2;
		ability2 = NULL;
	}
	if (clearAbility)
	{
		delete clearAbility;
		clearAbility = NULL;
	}
	if (settingsBG)
	{
		delete settingsBG;
		settingsBG = NULL;
	}
}

void SceneLobby::Init()
{
	SceneBase::Init();

	InitLights();

	// Calculating aspect ratio
	m_screenHeight = 100.f;

	//1920 x 1080
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = m_screenHeight;
	m_worldWidth = m_screenWidth;

	//Physics code here
	m_speed = 1.f;
	Math::InitRNG();

	// Game Manager
	gameManager = GameManager::GetInstance();
	// GO Manager
	goManager->Init();
	// Inventory 
	inventory->Init(this);

	
	// Unique Meshes
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	meshList[GEO_BG]->textureID = LoadTGA("Image/bg_lobby.tga");


	if (gameManager->getMachineStatus(1))
	{
		Button* part = ButtonFactory::createNoTextButton("machinePart1", 31.25, 37.5,
			5, 5, meshList[GEO_MACHINEPART_1]);
		machinePartsUIButtons.push_back(part);
	}
	if (gameManager->getMachineStatus(2))
	{
		Button* part = ButtonFactory::createNoTextButton("machinePart2", 47.5, 37.5,
			5, 5, meshList[GEO_MACHINEPART_2]);
		machinePartsUIButtons.push_back(part);
	}
	if (gameManager->getMachineStatus(3))
	{
		Button* part = ButtonFactory::createNoTextButton("machinePart3", 31.25, 21.25,
			5, 5, meshList[GEO_MACHINEPART_3]);
		machinePartsUIButtons.push_back(part);
	}
	if (gameManager->getMachineStatus(4))
	{
		Button* part = ButtonFactory::createNoTextButton("machinePart4", 47.5, 21.25,
			5, 5, meshList[GEO_MACHINEPART_4]);
		machinePartsUIButtons.push_back(part);
	}
	Button* machinePartSlot1 = ButtonFactory::createNoTextButton("machinePartSlot1", 31.25, 37.5,
		15, 15, machinePartsSlot);
	machinePartsUIButtons.push_back(machinePartSlot1);
	Button* machinePartSlot2 = ButtonFactory::createNoTextButton("machinePartSlot2", 47.5, 37.5,
		15, 15, machinePartsSlot);
	machinePartsUIButtons.push_back(machinePartSlot2);
	Button* machinePartSlot3 = ButtonFactory::createNoTextButton("machinePartSlot3", 31.25, 21.25,
		15, 15, machinePartsSlot);
	machinePartsUIButtons.push_back(machinePartSlot3);
	Button* machinePartSlot4 = ButtonFactory::createNoTextButton("machinePartSlot4", 47.5, 21.25,
		15, 15, machinePartsSlot);
	machinePartsUIButtons.push_back(machinePartSlot4);

	Button* machinePartsBG = ButtonFactory::createNoTextButton("machinePartBG", 40, 30,
		40, 40, machinePartsUIBG);
	machinePartsUIButtons.push_back(machinePartsBG);

	for (int i = 0; i < machinePartsUIButtons.size(); i++)
	{
		machinePartsUIButtons[i]->disable();
		buttonManager->addButton(machinePartsUIButtons[i]);
	}


	// ABILITY SELECTION UI SETTINGS
	Button* abilitySlot1 = ButtonFactory::createNoTextButton("abilitySlot1", 23.125, 37.5,
		15, 15, grapplingAbilityUI);
	abilityUIButtons.push_back(abilitySlot1);
	Button* abilitySlot2 = ButtonFactory::createNoTextButton("abilitySlot2", 39.375, 37.5,
		10, 10, dashAbilityUI);
	abilityUIButtons.push_back(abilitySlot2);
	Button* abilitySlot3 = ButtonFactory::createNoTextButton("abilitySlot3", 55.625, 37.5,
		10, 10, portalAbilityUI);
	abilityUIButtons.push_back(abilitySlot3);
	Button* abilitySlot4 = ButtonFactory::createNoTextButton("abilitySlot4", 23.125, 21.25,
		10, 10, recallAbilityUI);
	abilityUIButtons.push_back(abilitySlot4);
	Button* abilitySlot5 = ButtonFactory::createNoTextButton("abilitySlot5", 39.375, 21.25,
		10, 10, slowTimeAbilityUI);
	abilityUIButtons.push_back(abilitySlot5);
	Button* abilitySlot6 = ButtonFactory::createNoTextButton("abilitySlot6", 55.625, 21.25,
		10, 10, blackHoleAbilityUI);
	abilityUIButtons.push_back(abilitySlot6);
	Button* abilityDelete = ButtonFactory::createTextButton("abilityDelete", 40, 10, 15, 5, 4, 4, Color(1, 1, 1), "Clear", 4);
	abilityUIButtons.push_back(abilityDelete);
	Button* abilityBG = ButtonFactory::createNoTextButton("abilityBG", 40, 30,
		70, 50, abilityUIBG);
	abilityUIButtons.push_back(abilityBG);
	Button* abilitySelection1 = ButtonFactory::createNoTextButton("abilitySelection1", 100, 37.5,
		15, 15, ability1);
	buttonManager->addButton(abilitySelection1);
	Button* abilitySelection2 = ButtonFactory::createNoTextButton("abilitySelection2", 100, 37.5,
		15, 15, ability2);
	buttonManager->addButton(abilitySelection2);


	for (int i = 0; i < abilityUIButtons.size(); i++)
	{
		abilityUIButtons[i]->disable();
		buttonManager->addButton(abilityUIButtons[i]);
	}

	// SETTINGS MACHINE UI SETTINGS
	Button* ability1Text = ButtonFactory::createTextButton("ability_1_text", 35, 32, 10, 3, 0, 0, Color(1, 1, 1), "ABILITY 1", 3, CALIBRI);
	settingsUIButtons.push_back(ability1Text);
	Button* ability2Text = ButtonFactory::createTextButton("ability_2_text", 35, 27.5, 10, 3, 0, 0, Color(1, 1, 1), "ABILITY 2", 3, CALIBRI);
	settingsUIButtons.push_back(ability2Text);
	Button* interactText = ButtonFactory::createTextButton("interact_text", 35, 22.5, 10, 3, 0, 0, Color(1, 1, 1), "INTERACT", 3, CALIBRI);
	settingsUIButtons.push_back(interactText);
	Button* ability1_keybind = ButtonFactory::createTextButton("ability_1_keybind", 50, 35, 4, 4, 1, 3.5, Color(0.55, 0.55, 0.8), 
		std::string(1, gameManager->ABILITY_KEYBIND_1), 3, SUPERMARIO);
	settingsUIButtons.push_back(ability1_keybind);
	Button* ability2_keybind = ButtonFactory::createTextButton("ability_2_keybind", 50, 30, 4, 4, 1, 3.5, Color(0.55, 0.55, 0.8), 
		std::string(1, gameManager->ABILITY_KEYBIND_2), 3, SUPERMARIO);
	settingsUIButtons.push_back(ability2_keybind);
	Button* interact_keybind = ButtonFactory::createTextButton("interact_keybind", 50, 25, 4, 4, 1, 3.5, Color(0.55, 0.55, 0.8), 
		std::string(1, gameManager->INTERACT_KEYBIND), 3, SUPERMARIO);
	settingsUIButtons.push_back(interact_keybind);

	settingsSelectedButton = ability1_keybind;
	Button* settingsSelect = ButtonFactory::createNoTextButton("settings_selector", settingsSelectedButton->getOriginX(), settingsSelectedButton->getOriginY(),
		3, 3, ability1);
	settingsUIButtons.push_back(settingsSelect);
	Button* settingsUIBG = ButtonFactory::createNoTextButton("settingsBG", 40, 30, 40, 30, settingsBG);
	settingsUIButtons.push_back(settingsUIBG);
	for (int i = 0; i < settingsUIButtons.size(); i++)
	{
		settingsUIButtons[i]->disable();
		buttonManager->addButton(settingsUIButtons[i]);
	}

	showMachinePartsUI = false;
	showAbilityUI = false;
	showSettingsUI = false;
	

	Button* pressEtoInteract = ButtonFactory::createTextButton("pressEtoInteract", 33.5, 10, 15, 5, 3, 3, Color(1, 1, 1), "Press E to Interact", 4);
	buttonManager->addButton(pressEtoInteract);

	Button* selectAbilitiesFirst = ButtonFactory::createTextButton("selectAbilitiesFirst", 29, 8, 15, 5, 3, 3, Color(1, 1, 1), "Select your abilities first!", 4);
	buttonManager->addButton(selectAbilitiesFirst);
	buttonManager->deactivateButton("selectAbilitiesFirst");

	//Level Loading
	std::vector<GameObject*> tiles;
	if(LevelLoader::GetInstance()->LoadTiles("LOBBY", this->meshList, this->tileSize, tiles, gridLength, gridHeight))
		DEBUG_MSG("Level Did not load successfully");
	for (auto& go : tiles)
	{
		if (go->geoTypeID == GEOMETRY_TYPE::GEO_PLAYER_GIRL1)
		{
			player = new Player();
			player->active = true;
			player->scale = go->scale;
			player->pos = Vector3(91, 17, 0); //spawn player in center
			player->physics = go->physics->Clone();
			player->physics->SetInelasticity(0.99f);
			player->physics->SetIsBouncable(false);

			player->Init(&camera, Player::WASD, goManager, inventory);

			player->AddBottomSprite();
			player->bottomSprite->mesh = meshList[GEO_WALL];
			goManager->AddGO(player, true);

			DEBUG_MSG("From Phy Editor: " << player->scale);


			//Delete Grid Player
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_LOBBY_PORTAL_GRAVEYARD)
		{
			portal_graveyard = new LobbyPortal();
			portal_graveyard->active = true;
			portal_graveyard->scale = go->scale;
			portal_graveyard->pos = go->pos;
			portal_graveyard->physics = go->physics->Clone();
			portal_graveyard->Init(red);
			goManager->AddGO(portal_graveyard);

			//Delete portal
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_LOBBY_PORTAL_JUNGLE)
		{
			portal_jungle = new LobbyPortal();
			portal_jungle->active = true;
			portal_jungle->scale = go->scale;
			portal_jungle->pos = go->pos;
			portal_jungle->physics = go->physics->Clone();
			portal_jungle->Init(green);
			goManager->AddGO(portal_jungle);

			//Delete portal
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_LOBBY_PORTAL_OCEAN)
		{
			portal_ocean = new LobbyPortal();
			portal_ocean->active = true;
			portal_ocean->scale = go->scale;
			portal_ocean->pos = go->pos;
			portal_ocean->physics = go->physics->Clone();
			portal_ocean->Init(blue);
			goManager->AddGO(portal_ocean);

			//Delete portal
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_LOBBY_PORTAL_ROBOT)
		{
			portal_robot = new LobbyPortal();
			portal_robot->active = true;
			portal_robot->scale = go->scale;
			portal_robot->pos = go->pos;
			portal_robot->physics = go->physics->Clone();
			portal_robot->Init(purple);
			goManager->AddGO(portal_robot);

			//Delete portal
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_LOBBY_MACHINE1)
		{
			timeMachine = go;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_LOBBY_LEVELEDITOR)
		{
			wizardMachine = go;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_LOBBY_ABILITY_MACHINE)
		{
			abilityMachine = go;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_SETTINGS_MACHINE)
		{
			settingMachine = go;
		}
	}
	tiles.erase(std::remove(tiles.begin(), tiles.end(), nullptr), tiles.end());

	// Add all remaining tiles
	goManager->AddAllGO(tiles);


	// Dialogue
	if (gameManager->getGameState() == GameManager::GS_INTRO)
	{
		dialogueManager->AddDialogue(PLAYER, "Ouch my head hurts..Where am I? What happened to me..");
		dialogueManager->AddDialogue(PLAYER, "(You look around the room, but all you find is a funny looking machine.)");
		dialogueManager->AddDialogue(PLAYER, "Is this a time machine?");
		dialogueManager->AddDialogue(PLAYER, "??!!?!11!!?!");
		dialogueManager->AddDialogue(PLAYER, "(As you touch the machine it starts to shake violently before shattering into 4 pieces. They fly into corners of the room and each open up a portal)");
		dialogueManager->AddDialogue(PLAYER, "Oh no! I needed that to find out what happened, they seem to have created links to the worlds they flew into");
		dialogueManager->AddDialogue(PLAYER, "I need to go to each one and find each part to rebuild the machine and go back!");
		gameManager->setGameState(GameManager::GS_DEFAULT);
	}

	// Camera 
	camera.Init(Vector3(m_screenWidth * 0.5, m_screenHeight * 0.5, 1), Vector3(m_screenWidth * 0.5, m_screenHeight * 0.5, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
	//camera.SetFocusTarget(player->pos);

	sceneManager = SceneManager::GetInstance();

	gameManager->initAbilities(this, &camera, goManager, player);
	CSoundController::GetInstance()->PlaySoundByID(SOUND_TYPE::BG_LOBBY);
}

void SceneLobby::Update(double dt)
{
	SceneBase::Update(dt);
	//inventory->Update(dt);
	camera.Update(player->pos, dt);

	bool showEtoInteract = false;
	// button manager
	buttonManager->Update(dt);
	// TIME MACHINE INTERACTION
	if (abs(timeMachine->pos.y - player->pos.y) < 20 && abs(timeMachine->pos.x - player->pos.x) < 10)
	{
		if (!showMachinePartsUI)
			showEtoInteract = true;
		if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
		{
			showMachinePartsUI = !showMachinePartsUI;
			showAbilityUI = false;
			if (showMachinePartsUI)
			{
				for (int i = 0; i < machinePartsUIButtons.size(); i++)
				{
					buttonManager->activateButton(machinePartsUIButtons[i]->getName());
				}
			}
			else
			{
				for (int i = 0; i < machinePartsUIButtons.size(); i++)
				{
					buttonManager->deactivateButton(machinePartsUIButtons[i]->getName());
				}
			}
		}
	}

	// SETTINGS MACHINE INTERACTION
	if (abs(settingMachine->pos.y - player->pos.y) < 10 && abs(settingMachine->pos.x - player->pos.x) < 10)
	{
		if (!showSettingsUI)
			showEtoInteract = true;
		if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
		{
			showSettingsUI = !showSettingsUI;
			showAbilityUI = false;
			if (showSettingsUI)
			{
				for (int i = 0; i < settingsUIButtons.size(); i++)
				{
					buttonManager->activateButton(settingsUIButtons[i]->getName());
				}
			}
			else
			{
				for (int i = 0; i < settingsUIButtons.size(); i++)
				{
					buttonManager->deactivateButton(settingsUIButtons[i]->getName());
				}
			}
		}
	}


	//LEVELEDITOR MACHINE
	if (abs(wizardMachine->pos.y - player->pos.y) < 17 && abs(wizardMachine->pos.x - player->pos.x) < 7)
	{
		if (abs(wizardMachine->pos.y - player->pos.y) < 8)
		{
			showEtoInteract = true;
			if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
			{
				sceneManager->setScene(w_levelEditor);
				CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
			}
		}
		wizardMachine->invisible = false;
	}
	else
	{
		wizardMachine->invisible = true;
	}

	if (abs(abilityMachine->pos.y - player->pos.y) < 10 && abs(abilityMachine->pos.x - player->pos.x) < 10)
	{
		if (!showAbilityUI)
			showEtoInteract = true;
		if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
		{
			showAbilityUI = !showAbilityUI;
			if (showAbilityUI)
			{
				for (int i = 0; i < abilityUIButtons.size(); i++)
				{
					buttonManager->activateButton(abilityUIButtons[i]->getName());
				}
				buttonManager->activateButton("abilitySelection1");
				buttonManager->activateButton("abilitySelection2");
			}
			else
			{
				for (int i = 0; i < abilityUIButtons.size(); i++)
				{
					buttonManager->deactivateButton(abilityUIButtons[i]->getName());
				}
				buttonManager->deactivateButton("abilitySelection1");
				buttonManager->deactivateButton("abilitySelection2");
			}
		}
	}
	else
	{
		if (!showEtoInteract)
			showEtoInteract = false; //?????????????????????????????
	}

	// disable movement when showing UI
	if (showAbilityUI || showMachinePartsUI || showSettingsUI)
		player->physics->SetEnableUpdate(false);
	else
		player->physics->SetEnableUpdate(true);
	
	// BUTTON COLLISION RESPONSE
	for (auto& buttonCollide : buttonManager->getButtonsInteracted())
	{
		if (buttonCollide->justClicked)
		{
			for (int i = 0; i < abilityUIButtons.size() - 1; i++)
			{
				if (buttonCollide->buttonClicked->getName() == ("abilityDelete"))
				{
					selectedAbilities = false;
					gameManager->removeAbility(1);
					gameManager->removeAbility(2);
					buttonManager->getButtonByName("abilitySelection1")->setOrigin(100, buttonCollide->buttonClicked->getOriginY());
					buttonManager->getButtonByName("abilitySelection2")->setOrigin(100, buttonCollide->buttonClicked->getOriginY());
				}
				else if (buttonCollide->buttonClicked->getName() == ("abilitySlot" + std::to_string(i + 1)))
				{
					int abilityNum = -1;
					if (gameManager->getCurrAbility(1) == nullptr)
					{
						abilityNum = 1;
						buttonManager->getButtonByName("abilitySelection1")->setOrigin(buttonCollide->buttonClicked->getOriginX(), buttonCollide->buttonClicked->getOriginY());
					}
					else if (gameManager->getCurrAbility(1) != nullptr && gameManager->getCurrAbility(2) == nullptr)
					{
						std::cout << gameManager->getCurrAbility(1)->GetAbilityType() << " " << i << std::endl;
						if (gameManager->getCurrAbility(1)->GetAbilityType() != i)
						{
							selectedAbilities = true;
							abilityNum = 2;
							buttonManager->getButtonByName("abilitySelection2")->setOrigin(buttonCollide->buttonClicked->getOriginX(), buttonCollide->buttonClicked->getOriginY());
						}
					}

					if (abilityNum == -1)
						continue;

					switch (i)
					{
					case 0:
						gameManager->setAbility(abilityNum, ABILITY_GRAPPLER);
						break;
					case 1:
						gameManager->setAbility(abilityNum, ABILITY_DASH);
						break;
					case 2:
						gameManager->setAbility(abilityNum, ABILITY_PORTAL);
						break;
					case 3:
						gameManager->setAbility(abilityNum, ABILITY_RECALL);
						break;
					case 4:
						gameManager->setAbility(abilityNum, ABILITY_SLOWTIME);
						break;
					case 5:
						gameManager->setAbility(abilityNum, ABILITY_BLACKHOLE);
						break;
					}
				}
			}


			if (buttonCollide->buttonClicked->getName() == "ability_1_keybind"
				|| buttonCollide->buttonClicked->getName() == "ability_2_keybind"
				|| buttonCollide->buttonClicked->getName() == "interact_keybind")
			{
				settingsSelectedButton = buttonCollide->buttonClicked;
				buttonManager->getButtonByName("settings_selector")->setOrigin(settingsSelectedButton->getOriginX(),
					settingsSelectedButton->getOriginY());
			}
		}
	}

	bool besidePortal = false;
	
	// KEYBIND SETTING
	std::string entry = input->GetTypedEntry();
	if (showSettingsUI && entry.size() > 0)
	{
		char newChar = entry.back();
		settingsSelectedButton->setText(std::string(1, newChar));
		if (settingsSelectedButton->getName() == "ability_1_keybind")
		{
			gameManager->ABILITY_KEYBIND_1 = newChar;
		}
		else if (settingsSelectedButton->getName() == "ability_2_keybind")
		{
			gameManager->ABILITY_KEYBIND_2 = newChar;
		}
		else if (settingsSelectedButton->getName() == "interact_keybind")
		{
			gameManager->INTERACT_KEYBIND = newChar;
		}
	}

	// PORTALS
	if (((portal_graveyard->pos.y - 10.5) == player->pos.y) && (player->pos.x >= (portal_graveyard->pos.x - 4)) && (player->pos.x <= (portal_graveyard->pos.x + 4)))
	{
		besidePortal = true;
		if (selectedAbilities)
		{
			showEtoInteract = true;
			portal_graveyard->Open();
			if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
			{
				sceneManager->setScene(w_graveyard);
				CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
			}
		}
	}
	else {
		portal_graveyard->Close();
	}
	if (((portal_jungle->pos.y - 10.5) == player->pos.y) && (player->pos.x >= (portal_jungle->pos.x - 4)) && (player->pos.x <= (portal_jungle->pos.x + 4)))
	{
		besidePortal = true;
		if (selectedAbilities)
		{
			showEtoInteract = true;
			portal_jungle->Open();
			if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
			{
				sceneManager->setScene(w_jungle);
				CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
			}
		}
	}
	else {
		portal_jungle->Close();
	}
	if (((portal_ocean->pos.y - 10.5) == player->pos.y) && (player->pos.x >= (portal_ocean->pos.x - 4)) && (player->pos.x <= (portal_ocean->pos.x + 4)))
	{
		besidePortal = true;
		if (selectedAbilities)
		{
			showEtoInteract = true;
			portal_ocean->Open();
			if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
			{
				sceneManager->setScene(w_ocean);
				CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
			}
		}
	}
	else {
		portal_ocean->Close();
	}
	if (((portal_robot->pos.y - 10.5) == player->pos.y) && (player->pos.x >= (portal_robot->pos.x - 4)) && (player->pos.x <= (portal_robot->pos.x + 4)))
	{
		besidePortal = true;
		if (selectedAbilities)
		{
			showEtoInteract = true;
			portal_robot->Open();
			if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
			{
				sceneManager->setScene(w_robot);
				CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
			}
		}
	}
	else {
		portal_robot->Close();
	}

	//Show pop-up of if player has not selected abilities and want to enter portal
	if (besidePortal && !selectedAbilities)
		buttonManager->activateButton("selectAbilitiesFirst");
	else
		buttonManager->deactivateButton("selectAbilitiesFirst");

	portal_graveyard->Update(dt);
	portal_jungle->Update(dt);
	portal_ocean->Update(dt);
	portal_robot->Update(dt);


	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	if (!dialogueManager->Update(dt))
		goManager->Update(dt, &this->camera);

	if (showEtoInteract)
	{
		std::stringstream ss;
		ss << "Press " << gameManager->INTERACT_KEYBIND << " to interact";
		buttonManager->getButtonByName("pressEtoInteract")->setText(ss.str());
		buttonManager->activateButton("pressEtoInteract");
	}
	else
		buttonManager->deactivateButton("pressEtoInteract");
}

void SceneLobby::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
//	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projection.SetToOrtho(-1 * m_screenWidth * 0.5f, m_screenWidth * 0.5f, -1 * m_screenHeight * 0.5f, m_screenHeight * 0.5f, -10, 10);

	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();


	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
		//std::cout << "// DIRECTIONAL" << std::endl;
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		//std::cout << "// POINT" << std::endl;
	}

	if (lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}


	// BG
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, -0.01);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1); 
	RenderMesh(meshList[GEO_BG], true);
	modelStack.PopMatrix();

	goManager->Render(this);
	buttonManager->Render(this);
	dialogueManager->Render(this);

	std::ostringstream ss;
	//ss.str("");
	//ss << "LIGHT COLOR: " << Vector3(lights[0].color.r, lights[0].color.g, lights[0].color.b);
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 6);
	/*ss.str("");
	ss << "player vel: " << player->physics->GetVelocity();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 9);*/
	/*ss.str("");
	ss << "camera pos: " << camera.position;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 0, 12);*/



	// fps tings
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 0, 2);
}

void SceneLobby::InitLights()
{
	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 4;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 0.f, 1.f);

	lights[1].type = Light::LIGHT_SPOT;
	lights[1].position.Set(0, 0, 1);
	lights[1].color.Set(0, 0, 1);
	lights[1].power = 2;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 0.f, 1.f);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
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

	bLightEnabled = false;
}

void SceneLobby::CursorToWorldPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / m_screenWidth);
	y = h - y;
	y /= (h / m_screenHeight);
	x -= m_screenWidth * 0.5 - camera.position.x;
	y -= m_screenHeight * 0.5 - camera.position.y;

	theX = x;
	theY = y;
}

void SceneLobby::Exit()
{
	SceneBase::Exit();
	goManager->Exit();
	inventory->Clear();
	for (int i = 0; i < machinePartsUIButtons.size(); i++)
	{
		buttonManager->deleteButton(machinePartsUIButtons[i]);
	}
	machinePartsUIButtons.clear();
	for (int i = 0; i < abilityUIButtons.size(); i++)
	{
		buttonManager->deleteButton(abilityUIButtons[i]);
	}
	abilityUIButtons.clear();
	for (int i = 0; i < settingsUIButtons.size(); i++)
	{
		buttonManager->deleteButton(settingsUIButtons[i]);
	}
	settingsUIButtons.clear();
	CSoundController::GetInstance()->StopPlayingSoundByID(SOUND_TYPE::BG_LOBBY, 1, 0.5);
}

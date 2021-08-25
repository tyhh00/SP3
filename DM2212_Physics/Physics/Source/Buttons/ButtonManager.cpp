#include "ButtonManager.h"
#include "../Input.h"
#include "../Application.h"

/**
 * \brief Default constructor for Button Manager.
 * 
 */
ButtonManager::ButtonManager(float screenWidth, float screenHeight)
	: screenWidth(screenWidth)
	, screenHeight(screenHeight)
	, elapsed(0.f)
{
}

ButtonManager::ButtonManager()
	: screenWidth(80)
	, screenHeight(60)
	, elapsed(0.f)
{
}

/**
 * \brief Destructor for Button Manager, clearing up all buttons stored..
 * 
 */
ButtonManager::~ButtonManager() {
	for (auto& entry : this->buttons) {
		delete entry;
		entry = nullptr;
	}
	this->buttons.clear();
}

void ButtonManager::setScreenSize(float screenWidth, float screenHeight)
{
	if (screenWidth > 10 && screenHeight > 10)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
	}
}

//@DEPRECATED
void ButtonManager::Update(SceneBase* scene, double dt)
{
	Update(dt);
}

/**
 * \brief Updates all buttons, whether they are now clicked or not clicked.
 * 
 * \param scene - Scene that is currently using this Button Manager
 * \param dt - delta time of frame
 */
void ButtonManager::Update(double dt) {
	elapsed += dt;

	double xPos, yPos;
	bool mousePressed = Application::IsMousePressed(0);
	const static float CLICK_COOLDOWN = 0.5f;

	this->CursorPosition(xPos, yPos);

	std::vector<Button*> thisTickInteractedButtons;
	
	//Add all buttons interacted to list, used later in this code
	for (auto& button : buttons) {
		if (button->isEnabled() && button->isInRange(xPos, yPos)) {
			thisTickInteractedButtons.push_back(button);
		}
	}

	//Updates justClicked, justHovered
	for (auto& collisionEntry : buttonsInteractedThisTick) {
		if (std::find(thisTickInteractedButtons.begin(), thisTickInteractedButtons.end(), collisionEntry->buttonClicked) != thisTickInteractedButtons.end()) {
			collisionEntry->justHovered = false;
		}
		else { //Removes entries that have Buttons which are no longer collided with
			delete collisionEntry;
			collisionEntry = nullptr;
		}

		//If mouse is pressed and is still colliding
		if(collisionEntry != nullptr && mousePressed) {
			if(collisionEntry->clickTally == 0)
				collisionEntry->justClicked = true;
			else {
				collisionEntry->justClicked = false;
			}

			//Only says "Clicks" one tick every CLICK_COOLDOWN seconds. (So it dosent spam)
			if (!collisionEntry->isClicking) {
				if (collisionEntry->lastClickedTime + CLICK_COOLDOWN < elapsed) {
					collisionEntry->isClicking = true;
					collisionEntry->clickTally++;
				}
			}
			else {
				collisionEntry->isClicking = false;
			}
			
		}
		else if (collisionEntry != nullptr && !mousePressed) {
			collisionEntry->isClicking = false;
			collisionEntry->clickTally = 0;
		}
	}
	buttonsInteractedThisTick.erase(std::remove(buttonsInteractedThisTick.begin(), buttonsInteractedThisTick.end(), nullptr), buttonsInteractedThisTick.end());
	
	//Adds buttons found to main Vector used
	for (auto& button : thisTickInteractedButtons) {
		bool alrInList = false;
		for (auto& collision : buttonsInteractedThisTick) {
			if (collision->buttonClicked == button) {
				alrInList = true;
				break;
			}
		}
		if (!alrInList) {
			buttonsInteractedThisTick.push_back(new ButtonCollide(button, elapsed ));
		}
	}
}

/**
 * \brief Renders button on the screen based on current scene.
 * 
 * \param scene - Current scene that is active
 */
void ButtonManager::Render(SceneBase* scene)
{
	for (auto& button : buttons) {
		button->Render(scene);
	}
}

/**
 * \brief Get the list of interacted withs buttons. This is used for code execution after collision is detected.
 * 
 * \return a vector of buttons collided with mouse cursor in this current frame.
 */
std::vector<ButtonCollide*> ButtonManager::getButtonsInteracted() {
	return this->buttonsInteractedThisTick;
}

/**
 * \brief Calls delete on a button from this Button Manager.
 * 
 * \param button - Button to delete
 * \return whether the button was successfully deleted
 */
bool ButtonManager::deleteButton(Button* button) {
	bool deleted = false;
	for (auto& entry : buttons) {
		if (entry == button) {
			delete button;
			button = nullptr;
			deleted = true;
			break;
		}
	}
	for (auto& entry : buttonsInteractedThisTick) {
		if (entry->buttonClicked == button) {
			delete entry;
			entry = nullptr;
			break;
		}
	}
	buttons.erase(std::remove(buttons.begin(), buttons.end(), nullptr), buttons.end());
	buttonsInteractedThisTick.erase(std::remove(buttonsInteractedThisTick.begin(), buttonsInteractedThisTick.end(), nullptr), buttonsInteractedThisTick.end());
	return deleted;
}

/**
 * \brief Actives a button base of the button's String ID.
 * 
 * \param bN - Button's string ID
 * \return whether the button has found and activated.
 */
bool ButtonManager::activateButton(std::string bN) {
	for (auto& entry : buttons) {
		if (entry->getName() == bN) {
			entry->enable();
			return true;
		}
	}
	return false;
}

/**
 * \brief Deactivates a button base of the button's String ID.
 * 
 * \param bN - Button's string ID
 * \return whether the button has been found and deactivated.
 */
bool ButtonManager::deactivateButton(std::string bN) {
	for (auto& entry : buttons) {
		if (entry->getName() == bN) {
			entry->disable();
			return true;
		}
	}
	return false;
}

/**
 * \brief Gets a Button object by its name.
 * 
 * \param buttonName - The String ID of the button
 * \return nullptr if it does not exist, and the Button pointer object if it does
 */
Button* ButtonManager::getButtonByName(std::string buttonName) {
	for (auto& entry : buttons) {
		if (entry->getName() == buttonName) {
			return entry;
		}
	}
	return nullptr;
	
}

/**
 * \brief add a button into the Button Manager to be handled.
 * 
 * \param button - Button Object to be added
 */
void ButtonManager::addButton(Button* button) {
	for (auto& b : buttons)
	{
		if (b->getName() == button->getName())
			return;
	}
	buttons.push_back(button);
}

/**
 * \brief Gets all buttons currently in the button manager.
 * 
 * \return the vector of buttons currently in the button manager.
 */
std::vector<Button*> ButtonManager::getButtons() {
	return buttons;
}

void ButtonManager::CursorPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / screenWidth);
	y = h - y;
	y /= (h / screenHeight);
	theX = x;
	theY = y;
}
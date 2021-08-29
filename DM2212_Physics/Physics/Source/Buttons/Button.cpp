#include "Button.h"

/******************************************************************************/
/*!
\brief Button constructor

\param buttonName - string name of button
\param originX - origin X position of button
\param originY - origin Y position of button
\param quadSizeX - size of image
\param quadSizeY - size of image
\param quadTexture - geometry type of the button's background
*/
/******************************************************************************/
Button::Button(std::string buttonName, float originX, float originY, float quadSizeX, float quadSizeY, Mesh* quadTexture) : buttonName(buttonName), buttonType(REGULAR) {
	//this->scene = scene;
	this->text = nullptr;
	this->enabled = true;
	this->UIInfo.originX = originX;
	this->UIInfo.originY = originY;
	this->UIInfo.sizeX = quadSizeX;
	this->UIInfo.sizeY = quadSizeY;
	this->quadTexture = quadTexture;
}

BUTTON_TYPE Button::getButtonType()
{
    return buttonType;
}

float Button::getOriginX()
{
	return UIInfo.originX;
}

float Button::getOriginY()
{
	return UIInfo.originY;
}

/**
 * \brief Button Destructor
 * 
 */
Button::~Button() {
	if(text != nullptr)
	delete text;
}

/**
 * \brief gets the Button's text compoenent.
 * 
 * \param textPtr
 */
void Button::setTextObject(Text* textPtr) {
	if (text != nullptr) {
		delete text;
		text = nullptr;
	}
	text = textPtr;
}

/**
 * \brief set the Button's text component's text.
 * 
 * \param text - new text for the button
 */
void Button::setText(std::string text) {
	this->text->setTextString(text);
}

/**
 * \brief set the Button's text component's text but accepts an ostringstream instead of std::string.
 * 
 * \param text - new text for the button in ostringstream form.
 */
void Button::setText(std::ostringstream text) {
	this->text->setTextString(text.str());
}

/**
 * \brief gets the Text Component of the button.
 * 
 * \return nullptr if no text component created, Text object if it exists.
 */
Text* Button::getTextObject() {
	return text;
}

/**
 * \brief spawns a text object for the Button. Auto deletes the one created prior if already existed.
 * 
 * \param text - button text
 * \param txtColor - text's color
 * \param type - text's font type
 * \param textSize - text's font size
 */
void Button::spawnTextObject(std::string text, Color txtColor, FONTTYPE type, float textSize) {
	if (this->text != nullptr) {
		delete this->text;
	}
	this->text = new Text(txtColor, UIInfo, type, textSize);
	this->text->setTextString(text);
}

/**
 * \brief checks if the button has a text component.
 * 
 * \return true/false depending if the button has a text component
 */
bool Button::hasText() {
	return !(this->text == nullptr);
}

/**
 * \brief Renders the button on a scene.
 * 
 * \param scene - Scene to render this button on (Generally rendered in Game::activeScene)
 */
void Button::Render(SceneBase* scene, int z) {
	if (enabled) {
        if(quadTexture != nullptr)
		    scene->RenderMeshOnScreen(quadTexture, UIInfo.originX, UIInfo.originY, UIInfo.sizeX, UIInfo.sizeY, z);
		if (text != nullptr) {
			text->Render(scene, z);
		}
		
	}
}

/**
 * \brief check if the button itself is enabled.
 * 
 * \return - whether the button is enabled
 */
bool Button::isEnabled() {
	return enabled;
}

/**
 * \brief gets the String ID name of the button.
 * 
 * \return 
 */
std::string Button::getName() {
	return buttonName;
}

/**
 * \brief gets the UIInfo of the button consisting of its origin position and its size.
 * 
 * \return UIItem struct containing origin position and xSize, ySize values.
 */
UIItem Button::getUIInfo() {
	return UIInfo;
}

/**
 * \brief Sets the origin of the button. Used to move buttons around
 * 
 * \param x - new X pos
 * \param y - new Y pos
 */
void Button::setOrigin(float x, float y) {
	this->UIInfo.originX = x;
	this->UIInfo.originY = y;
	if (this->hasText()) {
		this->getTextObject()->updateUIInfo(this->UIInfo);
	}
}

/**
 * \brief used by Button Manager to check if cursor pos is in range of button.
 * 
 * \param x - Cursor x
 * \param y - Cursor y
 * \return - Whether the button is currently being hovered on by the mouse.
 */
bool Button::isInRange(double x, double y) {
	if (!enabled) return false;
	if(x >= UIInfo.originX - UIInfo.sizeX/2.0 && x < (UIInfo.originX + UIInfo.sizeX/2.0)
		&& y >= UIInfo.originY - UIInfo.sizeY / 2.0 && y < (UIInfo.originY + UIInfo.sizeY/2.0) ){
		return true;
	}
	return false;
}

/**
 * \brief Disables the button.
 * 
 */
void Button::disable() {
	enabled = false;
}

/**
 * \brief Enables the button.
 * 
 */
void Button::enable() {
	enabled = true;
}

/**
 * \brief sets the background image of the button.
 * 
 * \param type - the Image you want accessed through MeshHandler's list of Geometries loaded.
 */
void Button::setQuadImage(Mesh* type) {
	this->quadTexture = type;
}
#include "Text.h"
#include <fstream>

//Start Statics
Font Text::loadedFonts[FONT_COUNT];

/**
 * \brief Loads all the text fonts; Its character width, offsets etc.
 * 
 * \return whether the fonts are loaded. Returns false if already loaded
 */

bool Text::loadFont(FONTTYPE fType, std::string widthSheetName, SceneBase::GEOMETRY_TYPE typeID, float fontVerticalOffset)
{
    if (loadedFonts[fType].type == FONTTYPE::FONT_COUNT) //Aka not set
    {
        loadedFonts[fType].type = fType;
        loadedFonts[fType].geoType = typeID;
        loadedFonts[fType].verticalOffset = 1.0;
        LoadTextData(loadedFonts[fType].textWidth, "TextData//" + widthSheetName + ".csv");

        return true;
    }

    return false;
}

/**
 * \brief Checker function for if all Fonts have been generated.
 * 
 * \return true if fonts have been loaded, false if they haven't.
 */
bool Text::areFontsLoaded() {
    for (int i = 0; i < FONTTYPE::FONT_COUNT; ++i)
    {
        if (loadedFonts[i].type == FONTTYPE::FONT_COUNT)
            return false;
    }
    return true;
}

/**
 * \brief Get a Font object by the FONTTYPE enum.
 * 
 * \param type - Type of Font
 * \return - Actual font object that is needed for every text object.
 */
Font* Text::getFont(FONTTYPE type) {
    if (type != FONT_COUNT && loadedFonts[type].type != FONTTYPE::FONT_COUNT)
        return &loadedFonts[type];
    return nullptr;
}

/**
 * \brief Gets the string text message of a text.
 * 
 * \return the string text message of a text.
 */
std::string Text::getText() {
    return text.str();
}

/**
 * \brief Gets the color of the text.
 * 
 * \return the color of the text
 */
Color Text::getColor() {
    return color;
}

/**
 * \brief sets the color of the text.
 * 
 * \param color - the color for the text to change to.
 */
void Text::setColor(Color color) {
    this->color = color;
}

/**
 * \brief Character width loading system, through a CSV file for all the widths of each character for non-monospace fonts.
 * 
 * \param textDataArray - The Font objects array
 * \param textDataCSV - the csv file path location.
 */
void Text::LoadTextData(int textDataArray[], std::string textDataCSV) {
    std::ifstream data(textDataCSV);
    std::string line;
    int i = 0;

    if (!data.good())
        return;

    while (std::getline(data, line)) {
        textDataArray[i++] = std::stoi(line);
    }
}

//End Statics

/**
 * \brief Constructor of the text object.
 * 
 * \param color - Color of the text object
 * \param originX - Origin X position of the text object
 * \param originY - Origin Y position of the text object
 * \param quadSize - QuadSize of the text object
 * \param font - Type of font for the text
 * \param size - Font size
 */
Text::Text(Color color, float originX, float originY, float quadSize, FONTTYPE font, float size) {
    this->color = color;
    UIInfo.originX = originX;
    UIInfo.originY = originY;
    UIInfo.sizeX = quadSize;
    UIInfo.sizeY = quadSize;
    textFont = &loadedFonts[font];
    this->xOffset = 0.f;
    this->yOffset = 0.f;
    this->size = size;
    this->visible = true;
}

/**
 * \brief Another constructor for Text, but replaces most params with just UIInfo, this is to allow Buttons to share their UIInfos with the text object.
 * 
 * \param color - color of text
 * \param UIInfo - uiInfo of text
 * \param font - font type of text
 * \param size - font size of text
 */
Text::Text(Color color, UIItem UIInfo, FONTTYPE font, float size) {
    this->color = color;
    this->UIInfo = UIItem(UIInfo);
    textFont = &loadedFonts[font];
    this->size = size;
    this->visible = true;
}

/**
 * \brief set the UIInfo of the text object
 * 
 * \param info - UIItem struct containing origin x,y etc.
 */
void Text::updateUIInfo(UIItem info) {
    this->UIInfo = info;
}

/**
 * \brief Sets the text of the Text object.
 * 
 * \param txt - the new text
 */
void Text::setTextString(std::string txt) {
    text.str("");
    text.clear();
    text << txt;
}

/**
 * \brief Sets the Text of the Text Object through an ostringstream.
 * 
 * \param txt - the new text
 */
void Text::setTextStringStream(std::ostringstream txt) {
    text.str("");
    text.clear();
    text << txt.str();
}

/**
 * \brief Set the text offset from its UIInfo origin position.
 * This is used to allow Buttons to have text that are not directly at the border of a button.
 * 
 * \param x - xOffset
 * \param y - yOffset
 */
void Text::setTextOffsetFromTopLeft(float x, float y) {
    this->xOffset = x;
    this->yOffset = y;
}

/**
 * \brief whether Text is visible.
 * 
 * \return whether the text is visible
 */
bool Text::isVisible() {
    return visible;
}

/**
 * \brief set the visibility of the Text object.
 * 
 * \param visibility - whether it is visible
 */
void Text::setVisisble(bool visibility) {
    this->visible = visibility;
}

/**
 * \brief set the font size of the Text Object.
 * 
 * \param size - new font size
 */
void Text::setSize(float size) {
    this->size = size;
}

/**
 * \brief change font of Text Object.
 * 
 * \param type - new FONTTYPE to replace the old one with
 */
void Text::changeFont(FONTTYPE type) {
    if (loadedFonts[type].type != FONTTYPE::FONT_COUNT && type != FONT_COUNT) {
        this->textFont = getFont(type);
    }
}

/**
 * \brief Renders the text object on the associated scene that is parsed in.
 * 
 * \param scene - Scene to render text on
 */
void Text::Render(SceneBase* scene) {

    std::vector<std::string> splitVar;
    splitText(this->text.str(), '\n', splitVar);
    int i = 0;
    std::ostringstream ss;
    float startX, startY;
    startX = UIInfo.originX - UIInfo.sizeX * 0.5 + xOffset;
    startY = UIInfo.originY + UIInfo.sizeY * 0.5 - yOffset;
    for (auto& entry : splitVar) {
        ss.str("");
        ss.clear();
        ss << entry;
        scene->RenderTextOnScreen(scene->GetMesh(this->textFont->geoType), ss.str(), color, size, startX, startY - this->textFont->verticalOffset * size * i, textFont->textWidth, 256);
        i++;
    }

    //scene->RenderTextOnScreen(MeshHandler::getMesh(GEO_TEXT), this->text.str(), color, size, UIInfo.originX, UIInfo.originY);
}

/**
 * \brief Default destructor of Text Object.
 * 
 */
Text::~Text() {
    //Intentionally left blank
}

/**
 * \brief Splitter function to split strings by a delim.
 * 
 * \param txt - Text to split
 * \param delim - Splitting by character
 * \param out - out vector of the splitted string.
 */
void Text::splitText(std::string txt, char delim, std::vector<std::string>& out) {
    std::istringstream iss(txt);
    std::string item;
    while (std::getline(iss, item, delim)) {
        out.push_back(item);
    }
}
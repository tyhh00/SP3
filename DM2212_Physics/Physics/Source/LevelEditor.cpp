#include "LevelEditor.h"

#include <fstream>
#include <sstream>   

LevelEditor::LevelEditor()
{

}

LevelEditor::~LevelEditor()
{
	for (int i = 0; i < gridObjects.size(); i++)
	{
		if(gridObjects[i])
			delete gridObjects[i];
	}
}

void LevelEditor::Init()
{
	std::string mapToLoad = "LEVEL_1.txt";
	LoadMap(mapToLoad);
}

bool LevelEditor::LoadMap(std::string filename)
{
	std::string path = file_path + filename;
	std::ifstream fileStream(path.c_str(), std::ios::binary);
	if (!fileStream.is_open())
	{
		DEBUG_MSG("Impossible to open " << file_path << ". Are you in the right directory ?\n");
		return false;
	}
	int line = 0;
	bool loadingTiles = false;

	while (!fileStream.eof())
	{
		if (fileStream.fail())
		{
			DEBUG_MSG("File Stream failed at line: " << line);
			break;
		}
		char buf[1028];
		fileStream.getline(buf, 1028);

		line++;
		if (std::string(buf).empty()) continue;
		if (buf[0] == '#') continue;

		std::string key;
		
		key = "GridLength:";
		if (strncmp(key.c_str(), buf, key.size()) == 0)
		{
			char split[1028];
			strcpy_s(split, buf + (buf[key.size()] == ' ' ? key.size() + 1 : key.size()));
			gridLength = std::stoi(split);
		}

		key = "GridHeight:";
		if (strncmp(key.c_str(), buf, key.size()) == 0)
		{
			char split[1028];
			strcpy_s(split, buf + (buf[key.size()] == ' ' ? key.size() + 1 : key.size()));
			gridHeight = std::stoi(split);
		}

		key = "Tiles:";
		if (strncmp(key.c_str(), buf, key.size()) == 0)
		{
			loadingTiles = true;
			continue;
		}

		if (loadingTiles)
		{
			key = std::string(buf);
			std::vector<std::string> split = splitStr(key, ':');
			if (split.size() < 4) //GO_Type;Pos;Rot;Scale
			{
				DEBUG_MSG("Skipping " << line << " as it has a broken format in configuration");
				continue;
			}
			Vector3 pos, rot, scale;
			GameObject::GAMEOBJECT_TYPE type = static_cast<GameObject::GAMEOBJECT_TYPE>(std::stoi(split.at(0)));
			
			//Find Position
			std::vector<std::string> posSplit = splitStr(split.at(1), ',');
			if (posSplit.size() < 3)
			{
				DEBUG_MSG("Position " << split.at(1) << " attribute has a broken format in configuration. Skipping...");
				continue;
			}
			pos = Vector3(std::stoi(posSplit.at(0)), std::stoi(posSplit.at(1)), std::stoi(posSplit.at(2)));

			//Find Rotation
			std::vector<std::string> rotSplit = splitStr(split.at(2), ',');
			if (rotSplit.size() < 3)
			{
				DEBUG_MSG("Rotation " << split.at(2) << " attribute has a broken format in configuration. Skipping...");
				continue;
			}
			rot = Vector3(std::stoi(rotSplit.at(0)), std::stoi(rotSplit.at(1)), std::stoi(rotSplit.at(2)));

			//Find Scale
			std::vector<std::string> scaleSplit = splitStr(split.at(3), ',');
			if (scaleSplit.size() < 3)
			{
				DEBUG_MSG("Scale " << split.at(3) << " attribute has a broken format in configuration. Skipping...");
				continue;
			}
			scale = Vector3(std::stoi(scaleSplit.at(0)), std::stoi(scaleSplit.at(1)), std::stoi(scaleSplit.at(2)));
			if (scale.x <= 0 || scale.y <= 0 || scale.z <= 0)
			{
				DEBUG_MSG("Scale " << split.at(3) << " attribute cannot have values less than or equal to 0. Skipping...");
				continue;
			}




		}
		

	}
}




void LevelEditor::SaveMap()
{

}

void LevelEditor::ClearMap()
{

}

void LevelEditor::RemoveGO(GameObject* go)
{

}

void LevelEditor::Update(double dt)
{

}

void LevelEditor::Render()
{

}

void LevelEditor::Exit()
{

}

//Utilities
std::vector<std::string> splitStr(std::string str, char delimiter) {
	std::vector<std::string> internal;
	std::stringstream ss(str); // Turn the string into a stream. 
	std::string tok;

	while (std::getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}
#include "LevelLoader.h"
#include "Debug.h"
#include <fstream>
#include <sstream>

std::vector<std::string> splitStr(std::string str, char delimiter);

bool LevelLoader::LoadTiles(std::string filename, Mesh* meshList[], TileSetting* tileSize[], std::vector<GameObject*>& gridObjects, double& gridLength, double& gridHeight)
{
	std::string path = file_path + filename + ".txt";
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
			std::vector<std::string> split = splitStr(std::string(buf), ' ');
			gridLength = std::stod(split.at(1));
		}

		key = "GridHeight:";
		if (strncmp(key.c_str(), buf, key.size()) == 0)
		{
			std::vector<std::string> split = splitStr(std::string(buf), ' ');
			gridHeight = std::stod(split.at(1));
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
			Vector3 pos, normal, scale;
			SceneBase::GEOMETRY_TYPE type = static_cast<SceneBase::GEOMETRY_TYPE>(std::stoi(split.at(0)));
			if (!(type > SceneBase::GEOMETRY_TYPE::GEO_TILES_START && type < SceneBase::GEOMETRY_TYPE::GEO_TILES_END))
			{
				DEBUG_MSG("Not a valid Tile! GEOMETRY_TYPE ID: " << type << " is out of range of tiles selection");
				continue;
			}

			//Find Position
			std::vector<std::string> posSplit = splitStr(split.at(1), ',');
			if (posSplit.size() < 3)
			{
				DEBUG_MSG("Position " << split.at(1) << " attribute has a broken format in configuration. Skipping...");
				continue;
			}
			pos = Vector3(std::stod(posSplit.at(0)), std::stod(posSplit.at(1)), std::stod(posSplit.at(2)));

			//Find Rotation
			std::vector<std::string> rotSplit = splitStr(split.at(2), ',');
			if (rotSplit.size() < 3)
			{
				DEBUG_MSG("Normal " << split.at(2) << " attribute has a broken format in configuration. Skipping...");
				continue;
			}
			normal = Vector3(std::stod(rotSplit.at(0)), std::stod(rotSplit.at(1)), std::stod(rotSplit.at(2)));

			//Find Scale
			std::vector<std::string> scaleSplit = splitStr(split.at(3), ',');
			if (scaleSplit.size() < 3)
			{
				DEBUG_MSG("Scale " << split.at(3) << " attribute has a broken format in configuration. Skipping...");
				continue;
			}
			scale = Vector3(std::stod(scaleSplit.at(0)), std::stod(scaleSplit.at(1)), std::stod(scaleSplit.at(2)));
			if (scale.x <= 0 || scale.y <= 0 || scale.z <= 0)
			{
				DEBUG_MSG("Scale " << split.at(3) << " attribute cannot have values less than or equal to 0. Skipping...");
				continue;
			}

			GameObject* obj = new GameObject(GameObject::GO_TILE, meshList[type], type);
			obj->pos = pos;
			obj->physics->SetNormal(normal);
			obj->scale = scale * gridLength;
			obj->scale.x *= tileSize[type]->gridLength;
			obj->scale.y *= tileSize[type]->gridHeight;
			gridObjects.push_back(obj);
		}
	}
	DEBUG_MSG("Loaded Level " << filename << " with " << gridObjects.size() << " Tiles");
	return true;
}

std::vector<std::string> splitStr(std::string str, char delimiter) {
	std::vector<std::string> internal;
	std::stringstream ss(str); // Turn the string into a stream. 
	std::string tok;

	while (std::getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

LevelLoader::LevelLoader()
{

}

LevelLoader::~LevelLoader()
{

}

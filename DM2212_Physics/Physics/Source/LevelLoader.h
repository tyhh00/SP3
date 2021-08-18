#ifndef LEVELLOADER_H
#define LEVELLOADER_H
#include "SingletonTemplate.h"
#include <vector>
#include "GameObject.h"
#include "SceneBase.h"

class LevelLoader : public CSingletonTemplate<LevelLoader>
{
	friend CSingletonTemplate<LevelLoader>;
	const std::string file_path = "Levels\\";

public:
	bool LoadTiles(std::string mapName, Mesh* meshList[], TileSetting* tileSize[], std::vector<GameObject*>&, double&, double&);

protected:
	LevelLoader();
	~LevelLoader();
};


#endif
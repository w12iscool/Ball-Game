#pragma once
#include "raylib.h"
#include "box2d/box2d.h"

#include <vector>
#include <array>
#include <fstream>  
#include <iostream>  
#include <sstream>
#include <memory>

using Grid = std::array<std::array<int, 80>, 45>;

std::unique_ptr<Grid> returnMapFromCsv(std::ifstream& file);

extern struct map {
	std::unique_ptr<Grid> levelMap;
	bool canPlay = false;
	const char* levelNum;
	Vector2 startPos;

	map(std::unique_ptr<Grid> grid, bool canPlay, const char* levelNum, Vector2 startPos)
		: levelMap(std::move(grid)), canPlay(canPlay), levelNum(levelNum), startPos(startPos) {
	}

	map(map&& other) noexcept
		: levelMap(std::move(other.levelMap)),
		canPlay(other.canPlay),
		levelNum(other.levelNum),
		startPos(other.startPos)
	{
	}

	map(const map&) = delete;
	map& operator=(const map&) = delete;
};

extern map lvl1;

extern std::vector<map> levelArray;

enum tileType
{
	air,
	block,
	slant,
	goal,
	lava,
	spawn
};

class TileMaps
{
private:
	int m_tileSize{ 20 };
	int currentLevelNum{ 0 };
public:
	// Set member functions
	void InitLevels();
	void setCurrentLevelNum(int lvl);

	// Get member functions
	int getTileSize();
	int getCurrentLevelNum();

	// Important member functions
	void renderCurrentMap();
	void clearCurrentMap();
	void addLevel();
};

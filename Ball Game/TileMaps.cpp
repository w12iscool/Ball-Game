#include "TileMaps.h"

std::unique_ptr<Grid> returnMapFromCsv(std::ifstream& file)
{
	if (!file.is_open())
	{
		std::cerr << "Error opening file!\n";
		std::cout << "file not found";
	}
	else
	{
		std::cout << "file found\n";
	}
	std::unique_ptr<Grid> dataStorePtr =
		std::make_unique< std::array<std::array<int, 80>, 45>>();
	std::string line;
	int row = 0;

	while (std::getline(file, line) && row < 45)
	{
		std::stringstream ss(line);
		std::string cell;
		int col = 0;
		while (getline(ss, cell, ',') && col < 80)
		{
			(*dataStorePtr)[row][col] = std::stoi(cell);
			++col;
		}
		++row;
	}
	file.close();
	return dataStorePtr;
}

Vector2 returnPosFromTile(int y, int x)
{
	int tileSize = 20;
	int xPos;
	int yPos;
	for (int y1{ 0 }; y1 < 45; ++y1)
	{
		for (int x1{ 0 }; x1 < 80; ++x1)
		{
			xPos = x * tileSize;
			yPos = y * tileSize;
		}
	}
	return Vector2(xPos, yPos);
}

std::ifstream level1File("./level1.csv");

map lvl1
{
	returnMapFromCsv(level1File), false, "1", returnPosFromTile(31, 0)
};

extern std::vector<map> levelArray{};


void TileMaps::InitLevels()
{
	levelArray.push_back(std::move(lvl1));
}

void TileMaps::setCurrentLevelNum(int lvl)
{
	currentLevelNum = lvl;
}

int TileMaps::getTileSize()
{
	return m_tileSize;
}

int TileMaps::getCurrentLevelNum()
{
	return currentLevelNum;
}

void TileMaps::renderCurrentMap()
{
	for (int y{ 0 }; y < 45; ++y)
	{
		for (int x{ 0 }; x < 80; ++x)
		{
			std::cout << (*levelArray[currentLevelNum].levelMap)[y][x];
		}
		std::cout << "\n";
	}
}

void TileMaps::clearCurrentMap()
{
	for (int y{ 0 }; y < 45; ++y)
	{
		for (int x{ 0 }; x < 80; ++x)
		{
			(*levelArray[currentLevelNum].levelMap)[y][x] = 0;
		}
	}
}

void TileMaps::addLevel()
{
	++currentLevelNum;
}

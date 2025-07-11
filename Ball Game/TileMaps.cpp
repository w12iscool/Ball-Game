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
			int tile = (*levelArray[currentLevelNum].levelMap)[y][x];
			Color tileColor = BLUE;

			if (tile == tileType::block)
				tileColor = BLACK;
			if (tile == tileType::lava)
				tileColor = RED;
			if (tile == tileType::goal)
				tileColor = YELLOW;

			DrawRectangle(x * 16, y * 16, m_tileSize, m_tileSize, tileColor);

			if (tile == tileType::slant || tile == tileType::right_slant)
			{
				float offset = 6.0f;
				Vector2 topLeft = { (float)(x * 16), (float)(y * 16) + offset };
				Vector2 bottomRight = { (float)((x + 1) * 16), (float)((y + 1) * 16) + offset };
				Vector2 topRight = { (float)((x + 1) * 16), (float)(y * 16) + offset };
				Vector2 bottomLeft = { (float)(x * 16), (float)((y + 1) * 16) + offset };

				if (tile == tileType::slant)
					DrawLineEx(topLeft, bottomRight, 20, BLACK);
				else if (tile == tileType::right_slant)
					DrawLineEx(topRight, bottomLeft, 20, BLACK);

			}
		}
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

void TileMaps::setupBox2dTiles(b2WorldId& worldid)
{
	if (m_tilesInitialized)
		return;
	m_tilesInitialized = true;
	for (float y{ 0 }; y < 45; ++y)
	{
		for (float x{ 0 }; x < 80; ++x)
		{
			int tile = (*levelArray[currentLevelNum].levelMap)[y][x];

			if (tile == tileType::block)
			{
				b2BodyDef bodyDef = b2DefaultBodyDef();
				bodyDef.type = b2_staticBody;
				bodyDef.position = { ((x * 16) + 8.0f) / 20, ((y * 16) + 8.0f) / 20 };
				b2BodyId tileBodyId = b2CreateBody(worldid, &bodyDef);

				b2Polygon dynamicBox = b2MakeBox(0.5f, 0.5f);
				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = 1.0f;
				shapeDef.material.friction = 0.3f;
				b2CreatePolygonShape(tileBodyId, &shapeDef, &dynamicBox);
				bodies.push_back(tileBodyId);

				if (b2Body_IsValid)
				{
					std::cout << "initialized!";
				}
			}
		}
	}
}

std::vector<b2BodyId> TileMaps::returnBodies()
{
	return bodies;
}

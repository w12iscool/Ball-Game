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
	int tileSize = 16;
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
std::ifstream level2File("./level2.csv");
std::ifstream level3File("./level3.csv");

map lvl1
{
	returnMapFromCsv(level1File), false, "1", returnPosFromTile(31, 3)
};

map lvl2
{
	returnMapFromCsv(level2File), false, "2", returnPosFromTile(37, 4)
};

map lvl3
{
	returnMapFromCsv(level3File), false, "3", returnPosFromTile(40, 2)
};

extern std::vector<map> levelArray{};


void TileMaps::tpPlrToStartPos(Ball& plr)
{
	b2Body_SetTransform(plr.getBodyId(), b2Vec2{ levelArray[currentLevelNum].startPos.x / 20, levelArray[currentLevelNum].startPos.y / 20 }, b2Rot{ 1.0f });
}

void TileMaps::InitLevels()
{
	levelArray.push_back(std::move(lvl1));
	levelArray.push_back(std::move(lvl2));
	levelArray.push_back(std::move(lvl3));
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
			{
				tileColor = BLACK;
				DrawTexture((*blockTexture), x * 16, y * 16, WHITE);
			}
			else if (tile == tileType::lava)
			{
				tileColor = RED;
				DrawTexture((*lavaTexture), x * 16, y * 16, WHITE);
			}
			else if (tile == tileType::air)
			{
				tileColor = BLUE;
				DrawTexture((*skyTexture), x * 16, y * 16, WHITE);
			}
			else if (tile == tileType::physics_blocks)
			{
				tileColor = RAYWHITE;
				for (auto& physicsBox : dynamicBodies)
				{
					b2Vec2 position = b2Body_GetPosition(physicsBox);
					b2Rot rotation = b2Body_GetRotation(physicsBox);
					float angleRadians = atan2f(rotation.s, rotation.c);

					float angleDegrees = angleRadians * (180.0f / 3.14159265f);

					float x = position.x * 20;
					float y = position.y * 20;

					Rectangle tileRect = { x, y, m_tileSize, m_tileSize };
					DrawRectanglePro(tileRect, Vector2(m_tileSize / 2, m_tileSize / 2), angleDegrees, RAYWHITE);
				}
			}
			else
			{
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
}

void TileMaps::clearCurrentMap()
{
	for (int y{ 0 }; y < 45; ++y)
	{
		for (int x{ 0 }; x < 80; ++x)
		{
			bodies.clear();
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
	for (int i{ 0 }; i < bodies.size(); ++i)
	{
		b2DestroyBody(bodies[i]);
		std::cout << "cleaned: " << i << " \n";
	}
	bodies.clear();
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
				tileBodyId = b2CreateBody(worldid, &bodyDef);

				b2Polygon dynamicBox = b2MakeBox(0.5f, 0.5f);
				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = 1.0f;
				shapeDef.material.friction = 0.3f;
				b2CreatePolygonShape(tileBodyId, &shapeDef, &dynamicBox);
				bodies.push_back(tileBodyId);
				std::cout << bodies.size();
			}
			if (tile == tileType::physics_blocks)
			{
				b2BodyDef bodyDef = b2DefaultBodyDef();
				bodyDef.type = b2_dynamicBody;
				bodyDef.position = { ((x * 16) + 8.0f) / 20, ((y * 16) + 8.0f) / 20 };
				dynamicTileBodyId = b2CreateBody(worldid, &bodyDef);

				b2Polygon dynamicBox = b2MakeBox(0.5f, 0.5f);
				b2ShapeDef shapeDef = b2DefaultShapeDef();
				shapeDef.density = 0.2f;
				shapeDef.material.restitution = 0.5f;
				shapeDef.material.friction = 0.3f;
				b2CreatePolygonShape(dynamicTileBodyId, &shapeDef, &dynamicBox);
				b2Vec2 position = b2Body_GetPosition(dynamicTileBodyId);
				b2Rot rotation = b2Body_GetRotation(dynamicTileBodyId);
				float angleRadians = atan2f(rotation.s, rotation.c);
				float angleDegrees = angleRadians * (180.0f / 3.14159265f);
				float x = position.x * 20;
				float y = position.y * 20;
				Rectangle tileRect = { x, y, m_tileSize, m_tileSize };
				physicsBoxes.push_back(tileRect);
				dynamicBodies.push_back(dynamicTileBodyId);
				
				
			}
		}
	}
}

std::vector<b2BodyId> TileMaps::returnBodies()
{
	return bodies;
}

void TileMaps::handleCollision(Ball& plr, b2WorldId& worldId)
{
	bool isOnGround = false;
	for (int y{ 0 }; y < 45; ++y)
	{
		for (int x{ 0 }; x < 80; ++x)
		{
			int tile = (*levelArray[currentLevelNum].levelMap)[y][x];
			Rectangle plrHitbox{ (b2Body_GetPosition(plr.getBodyId()).x * 20) - 7.5, (b2Body_GetPosition(plr.getBodyId()).y * 20) + 17, 15, 4 };
			Rectangle plrEntireBodyHitbox{ (b2Body_GetPosition(plr.getBodyId()).x * 20) - 7.5, (b2Body_GetPosition(plr.getBodyId()).y * 20), 20, 20 };

			if (tile == tileType::block)
			{
				Rectangle tileRect{ x * 16, y * 16, m_tileSize, m_tileSize };

				if (CheckCollisionRecs(plrHitbox, tileRect))
				{
					isOnGround = true;
				}
			}

			if (tile == tileType::physics_blocks)
			{
				for (size_t i = 0; i < dynamicBodies.size(); ++i)
				{
					b2Vec2 position = b2Body_GetPosition(dynamicBodies[i]);

					float centerX = position.x * 20;
					float centerY = position.y * 20;

					physicsBoxes[i].x = centerX - (m_tileSize / 2.0f);
					physicsBoxes[i].y = centerY - (m_tileSize / 2.0f);

					if (CheckCollisionRecs(plrHitbox, physicsBoxes[i]))
					{
						isOnGround = true;
					}
				}
			}

			if (tile == tileType::lava)
			{
				Rectangle tileRect{ x * 16, y * 16, m_tileSize, m_tileSize };

				if (CheckCollisionRecs(plrEntireBodyHitbox, tileRect))
				{
					
					tpPlrToStartPos(plr);
					std::cout << b2Body_GetPosition(plr.getBodyId()).x * 20 << ", " << b2Body_GetPosition(plr.getBodyId()).y * 20 << "\n";
				}
			}

			if (tile == tileType::goal)
			{
				Rectangle tileRect{ x * 16, y * 16, m_tileSize, m_tileSize };

				if (currentLevelNum + 1 < levelArray.size())
				{
					if (CheckCollisionRecs(plrEntireBodyHitbox, tileRect))
					{
						levelArray[currentLevelNum + 1].canPlay = true;
						++currentLevelNum;
						tpPlrToStartPos(plr);
						m_tilesInitialized = false;
					}
				}
				
			}
			if (isOnGround) break;
		}
		plr.setIsOnGround(isOnGround);
	}
}

void TileMaps::InitTextures()
{
	(*blockTexture) = LoadTexture("./Resources/block.png");
	(*lavaTexture) = LoadTexture("./Resources/lava.jpg");
	(*skyTexture) = LoadTexture("./Resources/skyTexture.jpg");
}

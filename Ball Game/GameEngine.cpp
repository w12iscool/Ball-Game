#include "GameEngine.h"

void GameEngine::startUp()
{
	// Setup world
	b2SetLengthUnitsPerMeter(20);
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0.0f, 10.0f };
	worldId = b2CreateWorld(&worldDef);

	currentTileMap.InitLevels();
}

void GameEngine::update()
{
	
}

void GameEngine::render()
{
	currentTileMap.renderCurrentMap();
}

void GameEngine::shutDown()
{
}

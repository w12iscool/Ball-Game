#include "GameEngine.h"

std::vector<b2BodyId> tileMapBodiesVector;
void GameEngine::startUp()
{
	// Setup world
	b2SetLengthUnitsPerMeter(20);
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0.0f, 25.0f };
	worldId = b2CreateWorld(&worldDef);

	plr.setupBody(worldId);
	plr.initCamera();
	currentTileMap.InitLevels();
}

float timeStep = 1.0f / 60.0f;
int subStepCount = 4;


void GameEngine::update()
{
	
	b2World_Step(worldId, timeStep, 4);
	currentTileMap.setupBox2dTiles(worldId);
	plr.handleMovement(worldId);
}

void GameEngine::render()
{
	plr.cameraSetupBegin();
	currentTileMap.renderCurrentMap();
	plr.renderPlr();
}

void GameEngine::shutDown()
{
}

#include "GameEngine.h"

std::vector<b2BodyId> tileMapBodiesVector;
void GameEngine::startUp()
{
	// Init Textures
	currentTileMap.InitTextures();
	// Setup world
	b2SetLengthUnitsPerMeter(20);
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0.0f, 50.0f };
	worldId = b2CreateWorld(&worldDef);

	plr.setupBody(worldId);
	plr.initCamera();
	currentTileMap.InitLevels();
	b2Body_SetTransform(plr.getBodyId(), b2Vec2{ levelArray[currentTileMap.getCurrentLevelNum()].startPos.x / 20, levelArray[currentTileMap.getCurrentLevelNum()].startPos.y / 20}, b2Rot{1.0f});
}

float timeStep = 1.0f / 60.0f;
int subStepCount = 4;


void GameEngine::update()
{
	
	b2World_Step(worldId, timeStep, 4);
	currentTileMap.setupBox2dTiles(worldId);
	plr.handleMovement(worldId);
	currentTileMap.handleCollision(plr, worldId);
}

void GameEngine::render()
{
	plr.cameraSetupBegin();
	ClearBackground(BLACK);
	currentTileMap.renderCurrentMap();
	plr.renderPlr();
	
}

void GameEngine::shutDown()
{
	b2DestroyWorld(worldId);
}

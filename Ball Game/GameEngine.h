#pragma once
#include "raylib.h"
#include "box2d/box2d.h"
#include "TileMaps.h"
#include "Ball.h"

#include <iostream>

const int screenWidth{ 1280 };
const int screenHeight{ 720 };

class GameEngine
{
private:
	b2WorldId worldId = {};
	b2BodyId bodyId = {};

	TileMaps currentTileMap;
	Ball plr;
public:
	void startUp();
	void update();
	void render();
	void shutDown();
};


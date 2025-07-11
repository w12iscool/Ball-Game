#include "GameEngine.h"

int main()
{
	InitWindow(screenWidth, screenHeight, "Ball game");
	SetTargetFPS(60);
	GameEngine game;
	
	game.startUp();
	while (!WindowShouldClose())
	{
		game.update();
		BeginDrawing();
		ClearBackground(BLUE);
		game.render();
		EndDrawing();
	}
	game.shutDown();
	CloseWindow();
	return 0;
}
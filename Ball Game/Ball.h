#pragma once

#include "raylib.h"
#include "box2d/box2d.h"

#include <vector>
#include <iostream>

class Ball
{
private:
	Vector2 m_pos = Vector2(0, 0);
	float m_radius{ 15 };
	Color m_color{ RED };
	b2BodyId m_bodyId = {};
	Camera2D m_camera;
public:
	// Get member functions
	Vector2 getPos();
	float getRadius();
	Color getColor();

	// Important member functions
	void setupBody(b2WorldId& worldId);
	void renderPlr();
	void cameraSetupBegin();
	void initCamera();
	void handleMovement(b2WorldId& worldId);
};


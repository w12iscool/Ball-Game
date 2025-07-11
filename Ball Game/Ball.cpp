#include "Ball.h"

Vector2 Ball::getPos()
{
	return m_pos;
}

float Ball::getRadius()
{
	return m_radius;
}

Color Ball::getColor()
{
	return m_color;
}

void Ball::setupBody(b2WorldId& worldId)
{
	
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = { m_pos.x / 20, m_pos.y / 20 };
	m_bodyId = b2CreateBody(worldId, &bodyDef);

	b2Circle circle;
	circle.center = { m_pos.x / 20, m_pos.y / 20 };
	circle.radius = m_radius / 20;
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.material.friction = 0.0f;
	shapeDef.material.restitution = 0.5f;
	b2CreateCircleShape(m_bodyId, &shapeDef, &circle);
}

void Ball::renderPlr()
{
	DrawCircle(b2Body_GetPosition(m_bodyId).x * 20, b2Body_GetPosition(m_bodyId).y * 20, m_radius, RED);
}

void Ball::cameraSetupBegin()
{
	m_camera.target = Vector2(b2Body_GetPosition(m_bodyId).x * 20, b2Body_GetPosition(m_bodyId).y * 20);
	BeginMode2D(m_camera);
}

void Ball::initCamera()
{
	m_camera = {
		.offset = Vector2(1280 / 2, 720 / 2),
		.rotation = 0,
		.zoom = 2.5,
	};
}

void Ball::handleMovement(b2WorldId& worldId)
{
	if (IsKeyDown(KEY_D))
	{
		b2Vec2 force = { 10.0f, 0.0f };
		b2Body_ApplyForceToCenter(m_bodyId, force, true);
	}
	if (IsKeyDown(KEY_A))
	{
		b2Vec2 force = { -10.0f, 0.0f };
		b2Body_ApplyForceToCenter(m_bodyId, force, true);
	}
	if (IsKeyPressed(KEY_W))
	{
		b2Vec2 force = { 0.0f, -30.0f };
		b2Body_ApplyLinearImpulseToCenter(m_bodyId, force, true);
		
	}
}

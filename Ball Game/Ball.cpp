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

b2BodyId Ball::getBodyId()
{
	return m_bodyId;
}

bool Ball::getIsOnground()
{
	return m_isOnGround;
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
	shapeDef.density = 0.5f;
	shapeDef.material.friction = 10.0f;
	m_shapeId = b2CreateCircleShape(m_bodyId, &shapeDef, &circle);
	
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
	b2Body_SetLinearDamping(m_bodyId, 1.0f);

	if (IsKeyDown(KEY_D))
	{
		b2Vec2 force = { 0.3f, 0.0f };
		b2Body_ApplyLinearImpulseToCenter(m_bodyId, force, true);
	}
	if (IsKeyDown(KEY_A))
	{
		b2Vec2 force = { -0.3f, 0.0f };
		b2Body_ApplyLinearImpulseToCenter(m_bodyId, force, true);
	}
	if (IsKeyPressed(KEY_W) && m_isOnGround)
	{
		b2Vec2 force = { 0.0f, -20.0f };
		b2Body_ApplyLinearImpulseToCenter(m_bodyId, force, true);
	}
}

void Ball::setIsOnGround(bool onGround)
{
	m_isOnGround = onGround;
}

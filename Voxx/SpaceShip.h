#pragma once
#include"HealthBar.h"

class SpaceShip
{
private:
	HealthBar health_bar;
	ImageSprite sprite;
public:
	enum class Type
	{
		Ally,
		Enemy,
	};
private:
	Type type;
public:
	SpaceShip(CoreEngine& engine, Type type)
		:
		type(type),
		health_bar(engine, 1000),
		sprite(engine.CreateSprite(Image{ type == SpaceShip::Type::Ally ? "media/space_ship.png" : "media/Enemy_ship.png"}))
	{}
private:
	float rotation = 0.0f;
	DirectX::XMVECTOR direction = DirectX::XMVectorZero();
public:
	void MoveTo(unsigned int x, unsigned int y)
	{
		sprite.SetPosition(DirectX::XMVectorSet(x, y, 0, 1));
		health_bar.SetPosition(DirectX::XMVectorSet(x, y, 0, 1));
	}
	void RotateTowards(DirectX::XMVECTOR pos)
	{
		auto dir = type == Type::Ally ? DirectX::XMVectorSubtract(pos , sprite.GetPosition()) : DirectX::XMVectorSubtract(sprite.GetPosition() , pos);
		auto angle = std::atan2(DirectX::XMVectorGetY(dir), DirectX::XMVectorGetX(dir));

		sprite.SetTransformation(DirectX::XMMatrixRotationZ(angle + 1.6));
		rotation = angle;
		direction = DirectX::XMVector2Normalize(dir);
	}
	void AddHealth(int amount)
	{
		health_bar.SetHealth(std::clamp((int)health_bar.GetHealth() + amount, 0, 1000));
	}
	bool Died()
	{
		return health_bar.GetHealth() == 0;
	}
public:
	auto GetPosition()
	{
		return sprite.GetPosition();
	}
	auto GetHeight()
	{
		return sprite.GetHeight();
	}
	auto GetWidth()
	{
		return sprite.GetWidth();
	}
	float GetRotation()
	{
		return rotation;
	}
	auto GetDirection()
	{
		return direction;
	}
public:
	void Reset()
	{
		health_bar.SetHealth(1000);
	}
public:
	void Draw(CoreEngine& engine)
	{
		sprite.Draw(engine);
		health_bar.Draw();
	}
};
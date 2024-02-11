#pragma once
#include <array>
#include "StatusBar.h"
#include "SkillEquipment.h"

class SpaceShip
{
private:
	StatusBar health_bar;
	StatusBar skillpoint_bar;
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
	std::unique_ptr<SkillEquipment> Skill;
public:
	SpaceShip(CoreEngine& engine, Type type)
		:
	type(type),
	health_bar(engine,1000),
	skillpoint_bar(engine, 100, {250 , 50 , 50} , 100 , 10),
	Skill( nullptr ),
	sprite(engine.CreateSprite(Image{ type == SpaceShip::Type::Ally ? "media/space_ship.png" : "media/Enemy_ship.png" }))
	{}
private:
	DirectX::XMVECTOR direction = DirectX::XMVectorZero();
public:
	void MoveTo(unsigned int x, unsigned int y)
	{
		sprite.SetPosition(DirectX::XMVectorSet(x, y, 0, 1));
		skillpoint_bar.SetPosition(DirectX::XMVectorSet(x, y - 5, 0, 1));
		health_bar.SetPosition(DirectX::XMVectorSet(x, y + 5, 0, 1));
	}
	void RotateTowards(DirectX::XMVECTOR pos)
	{
		//auto dir = type == Type::Ally ? DirectX::XMVectorSubtract(pos, sprite.GetPosition()) : DirectX::XMVectorSubtract(sprite.GetPosition(), pos);
		auto dir = DirectX::XMVectorSubtract(pos, sprite.GetPosition());
		auto angle = std::atan2(DirectX::XMVectorGetY(dir), DirectX::XMVectorGetX(dir));

		sprite.SetTransformation(DirectX::XMMatrixRotationZ(angle + 1.6));
		direction = DirectX::XMVector2Normalize(dir);
	}
	void AddHealth(int amount)
	{
		health_bar.SetStatus(std::clamp((int)health_bar.GetStatus() + amount, 0, 1000));
	}
	void AddSkillPoint(int amount)
	{
		skillpoint_bar.SetStatus(std::clamp((int)skillpoint_bar.GetStatus() + amount, 0, 100));
	}
	unsigned int GetSkillPoint() const
	{
		return skillpoint_bar.GetStatus();
	}
	bool Died()
	{
		return health_bar.GetStatus() == 0;
	}
public:
	auto GetPosition() const
	{
		return sprite.GetPosition();
	}
	auto GetHeight() const
	{
		return sprite.GetHeight();
	}
	auto GetWidth() const
	{
		return sprite.GetWidth();
	}
	auto GetDirection() const
	{
		return direction;
	}
public:
	void Reset()
	{
		health_bar.SetStatus(1000);
		skillpoint_bar.SetStatus(100);
	}
public:
	void Draw(CoreEngine& engine)
	{
		sprite.Draw(engine);
		health_bar.Draw();
		skillpoint_bar.Draw();
	}
};
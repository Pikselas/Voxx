#pragma once
#include"Engine/CoreEngine.h"

class Projectile
{
protected:
	DirectX::XMVECTOR control_parameter;
protected:
	ImageSprite projectile;
protected:
	Projectile(ImageSprite sprite, DirectX::XMVECTOR control_parameter , DirectX::XMVECTOR position)
		: 
	control_parameter(control_parameter), projectile(sprite)
	{
		projectile.SetPosition(position);
	}
public:
	void Draw(RenderCommandEngine& engine)
	{
		projectile.Draw(engine);
	}
	DirectX::XMVECTOR GetPosition() const
	{
		return projectile.GetPosition();
	}
	std::pair<unsigned int, unsigned int> GetSideLength() const
	{
		return { projectile.GetWidth() , projectile.GetHeight() };
	}
public:
	virtual void UpdatePosition() = 0;
};
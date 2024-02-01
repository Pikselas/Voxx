#pragma once
#include"Engine/CoreEngine.h"

class Projectile
{
protected:
	DirectX::XMVECTOR control_parameter;
protected:
	ImageSprite projectile;
protected:
	Projectile(CoreEngine& engine, std::filesystem::path path , DirectX::XMVECTOR control_parameter , DirectX::XMVECTOR position)
		: 
	control_parameter(control_parameter), projectile(engine.CreateSprite(Image{path}))
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
	virtual void Update() = 0;
};
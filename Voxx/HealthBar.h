#pragma once
#include"Engine/CoreEngine.h"

class HealthBar
{
private:
	ImageSprite bar;
private:
	CoreEngine& engine;
private:
	const unsigned int max_health;
	unsigned int health;
public:
	HealthBar(CoreEngine& engine, unsigned int max_health, unsigned int width = 100, unsigned int height = 20) : max_health(max_health), engine(engine), health(max_health)
	{
		Image bar_(width, height);
		bar_.Clear({ 0 , 0 , 255 });
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				bar_.SetPixel(i, j, { 200 , 255 , 0 });
				bar_.SetPixel(i, height - j, { 200 , 255 , 0 });
			}
		}
		bar = engine.CreateSprite(bar_);
	}
public:
	void SetHealth(unsigned int health)
	{
		if (health > max_health)
		{
			health = max_health;
		}

		this->health = health;

		float p = (float)health / (float)max_health;
		auto size = (bar.GetWidth() - (bar.GetWidth() * p)) / 2;
		//auto pos = DirectX::XMVector3Transform(bar.GetPosition(), DirectX::XMMatrixScaling(p, 1.0f, 1.0f));
		//pos = DirectX::XMVectorSubtract(pos, bar.GetPosition());
		auto pos = DirectX::XMVectorSet(-size, 0, 0, 1);
		//pos = DirectX::XMVectorDivide(pos, DirectX::XMVectorSet(2.0f, 1.0f, 1.0f, 1.0f));

		bar.SetTransformation(DirectX::XMMatrixScaling(p, 1.0f, 1.0f) * DirectX::XMMatrixTranslationFromVector(pos));
	}
	unsigned int GetHealth()
	{
		return health;
	}
	void SetPosition(DirectX::XMVECTOR position)
	{
		bar.SetPosition(position);
		//SetHealth(health);
	}
public:
	void Draw()
	{
		bar.Draw(engine);
	}
};
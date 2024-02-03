#pragma once
#include"Engine/CoreEngine.h"

class StatusBar
{
private:
	ImageSprite bar;
private:
	CoreEngine& engine;
private:
	const unsigned int max_stat;
	unsigned int current_stat;
public:
	StatusBar(CoreEngine& engine, unsigned int max_stat, ColorType color = { 0 , 0 , 255 } ,unsigned int width = 100, unsigned int height = 20) : max_stat(max_stat), engine(engine), current_stat(max_stat)
	{
		Image bar_(width, height);
		bar_.Clear(color);
		bar = engine.CreateSprite(bar_);
	}
public:
	void SetStatus(unsigned int stat)
	{
		if (stat > max_stat)
		{
			stat = max_stat;
		}

		current_stat = stat;

		float p = (float)current_stat / (float)max_stat;
		auto size = (bar.GetWidth() - (bar.GetWidth() * p)) / 2;
		//auto pos = DirectX::XMVector3Transform(bar.GetPosition(), DirectX::XMMatrixScaling(p, 1.0f, 1.0f));
		//pos = DirectX::XMVectorSubtract(pos, bar.GetPosition());
		auto pos = DirectX::XMVectorSet(-size, 0, 0, 1);
		//pos = DirectX::XMVectorDivide(pos, DirectX::XMVectorSet(2.0f, 1.0f, 1.0f, 1.0f));

		bar.SetTransformation(DirectX::XMMatrixScaling(p, 1.0f, 1.0f) * DirectX::XMMatrixTranslationFromVector(pos));
	}
	unsigned int GetStatus() const
	{
		return current_stat;
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
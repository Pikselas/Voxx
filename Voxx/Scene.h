#pragma once
#include<deque>
#include<stack>
#include "Engine/CoreEngine.h"
class Scene
{
private:
	CoreEngine& engine;
private:
	struct path_component
	{
		float scale_x;
		float scale_y;
		ImageSprite component;
	};
private:
	ImageSprite sky;
	ImageSprite space_ship;
	ImageSprite path_comp;
private:
	std::deque<path_component> path_components;
public:
	Scene(CoreEngine& engine) : engine(engine){}
public:
	void Render()
	{
		sky.Draw(engine);

		std::stack<path_component> new_path_components;
		int path_size = path_components.size();
		while (path_size-- != 0)
		{
			auto pathcomp = path_components.front();
			path_components.pop_front();

			pathcomp.component.Draw(engine);
			pathcomp.component.SetPosition(DirectX::XMVectorAdd(pathcomp.component.GetPosition(), DirectX::XMVectorSet(0, 1, 0, 0)));
			pathcomp.scale_x += 0.005;
			pathcomp.scale_y += 0.0025;
			pathcomp.component.SetTransformation(DirectX::XMMatrixScaling(pathcomp.scale_x, pathcomp.scale_y, 1.0f));

			if (DirectX::XMVectorGetY(pathcomp.component.GetPosition()) > 700)
			{
				pathcomp.component.SetPosition(DirectX::XMVectorSet(509, 470, 0, 0));
				pathcomp.scale_x = 0.65;
				pathcomp.scale_y = 0.3;
				pathcomp.component.SetTransformation(DirectX::XMMatrixScaling(pathcomp.scale_x, pathcomp.scale_y, 1.0));
				new_path_components.push(pathcomp);
			}
			else
			{
				path_components.push_back(pathcomp);
			}
		}

		while(!new_path_components.empty())
		{
			path_components.push_front(new_path_components.top());
			new_path_components.pop();
		}

		space_ship.Draw(engine);
	}
	void SetSky(ImageSprite sky)
	{
		this->sky = sky;
		this->sky.SetPosition(DirectX::XMVectorSet(509, 240, 0, 0));
	}
	void SetSpaceShip(ImageSprite space_ship)
	{
		this->space_ship = space_ship;
	}
	void SetPath(ImageSprite path_comp)
	{
		int pos_y = 350;
		float scale_x = 0.65 / 5;
		float scale_y = 0.3 / 5;

		for (int i = 0; i < 15; ++i)
		{
			auto comp = path_comp;
			comp.SetPosition(DirectX::XMVectorSet(509, pos_y, 0, 0));
			comp.SetTransformation(DirectX::XMMatrixScaling(scale_x, scale_y, 1.0));
			path_components.emplace_back(scale_x, scale_y, comp);

			pos_y += 20;
			scale_x += 0.10;
			scale_y += 0.05;

		}
		this->path_comp = path_comp;
	}
public:
	void SpaceShipController(CustomWindow& window)
	{
		space_ship.SetPosition(DirectX::XMVectorSet(window.mouse.GetX(), 580, 0, 0));
		auto middle = DirectX::XMVectorSet(509, 350, 0, 0);
		auto pos = space_ship.GetPosition();
		auto dist = DirectX::XMVectorSubtract(middle, pos);
		auto angle = std::atan2(DirectX::XMVectorGetY(dist), DirectX::XMVectorGetX(dist));
		space_ship.SetTransformation(DirectX::XMMatrixRotationZ(angle + 1.6));
	}
};
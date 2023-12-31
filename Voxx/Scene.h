#pragma once
#include<stack>
#include<queue>
#include "Engine/CoreEngine.h"

#include"SpaceShip.h"

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
	struct Bullet
	{
		DirectX::XMVECTOR velocity;
		ImageSprite bullet;
	};
private:
	ImageSprite sky;
	ImageSprite bullet;
	ImageSprite enemy_bullet;
	ImageSprite path_comp;
private:
	SpaceShip player_ship;
	SpaceShip enemy_ship;
private:
	std::deque<path_component> path_components;
private:
	std::queue<Bullet> bullets;
	std::queue<Bullet> enemy_bullets;
public:
	Scene(CoreEngine& engine) : engine(engine), player_ship(engine, SpaceShip::Type::Ally), enemy_ship(engine, SpaceShip::Type::Enemy)
	{
		player_ship.MoveTo(509, 580);
		enemy_ship.MoveTo(509, 250);
	}
private:
	bool IsColliding(DirectX::XMVECTOR pos1 , unsigned int half_w1 , unsigned int half_h1, DirectX::XMVECTOR pos2 , unsigned int half_w2 , unsigned int half_h2)
	{
		auto dist = DirectX::XMVectorSubtract(pos1, pos2);
		auto dist_x = std::abs(DirectX::XMVectorGetX(dist));
		auto dist_y = std::abs(DirectX::XMVectorGetY(dist));

		return dist_x < half_w1 + half_w2 && dist_y < half_h1 + half_h2;
	}
public:
	std::function<void()> OnLose = []() {};
	std::function<void()> OnWin = []() {};
public:
	void Render()
	{
		sky.Draw(engine);

		std::stack<path_component> new_path_components;
		int size_ = path_components.size();
		while (size_-- != 0)
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

		size_ = bullets.size();
		while (size_-- != 0)
		{
			auto bullet = bullets.front();
			bullets.pop();

			bullet.bullet.Draw(engine);

			auto pos = bullet.bullet.GetPosition();
			pos = DirectX::XMVectorAdd(pos, bullet.velocity);
			bullet.bullet.SetPosition(pos);

			bool OutOfWindow = DirectX::XMVectorGetX(DirectX::XMVectorLess(pos, DirectX::XMVectorSet(0, 0, 0, 0)));
			OutOfWindow |= (bool)DirectX::XMVectorGetX(DirectX::XMVectorGreater(pos, DirectX::XMVectorSet(1018, 700, 0, 0)));

			auto Colliding = IsColliding(pos, bullet.bullet.GetWidth()/2, bullet.bullet.GetHeight() /2, enemy_ship.GetPosition(), enemy_ship.GetWidth() / 2, enemy_ship.GetHeight() / 2);
			
			if (Colliding)
			{
				enemy_ship.AddHealth(-5);
				continue;
			}

			if (OutOfWindow)
			{
				continue;
			}

			bullets.push(bullet);
		}

		size_ = enemy_bullets.size();
		while (size_-- != 0)
		{
			auto bullet = enemy_bullets.front();
			enemy_bullets.pop();

			bullet.bullet.Draw(engine);

			auto pos = bullet.bullet.GetPosition();
			pos = DirectX::XMVectorAdd(pos, bullet.velocity);
			bullet.bullet.SetPosition(pos);

			bool OutOfWindow = DirectX::XMVectorGetX(DirectX::XMVectorLess(pos, DirectX::XMVectorSet(0, 0, 0, 0)));
			OutOfWindow |= (bool)DirectX::XMVectorGetX(DirectX::XMVectorGreater(pos, DirectX::XMVectorSet(1018, 700, 0, 0)));

			auto Colliding = IsColliding(pos, bullet.bullet.GetWidth() / 2, bullet.bullet.GetHeight() / 2, player_ship.GetPosition(), player_ship.GetWidth() / 2, player_ship.GetHeight() / 2);

			if (Colliding)
			{
				player_ship.AddHealth(-5);
				continue;
			}

			if (OutOfWindow)
			{
				continue;
			}
			enemy_bullets.push(bullet);
		}

		enemy_ship.Draw(engine);
		player_ship.Draw(engine);

		if(enemy_ship.Died())
		{
			OnWin();
		}
		else if(player_ship.Died())
		{
			OnLose();
		}
	}
	void SetSky(ImageSprite sky)
	{
		this->sky = sky;
		this->sky.SetPosition(DirectX::XMVectorSet(509, 240, 0, 0));
	}
	void SetBullet(ImageSprite bullet)
	{
		this->bullet = bullet;
	}
	void SetEnemyBullet(ImageSprite bullet)
	{
		this->enemy_bullet = bullet;
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
	void SetEnemyShipPosition(int x)
	{
		enemy_ship.MoveTo(x, 250);
		enemy_ship.RotateTowards(player_ship.GetPosition());
		player_ship.RotateTowards(enemy_ship.GetPosition());
	}
public:
	void Reset()
	{
		bullets = std::queue<Bullet>();
		enemy_bullets = std::queue<Bullet>();

		player_ship.Reset();
		enemy_ship.Reset();
	}
public:
	void SpaceShipController(CustomWindow& window)
	{
		player_ship.MoveTo(window.mouse.GetX(), 580);
		player_ship.RotateTowards(enemy_ship.GetPosition());
		enemy_ship.RotateTowards(player_ship.GetPosition());
	}
	void FireBullet()
	{
		auto bullet1 = bullet;
		auto bullet2 = bullet;

		bullet1.SetPosition(DirectX::XMVectorSet(DirectX::XMVectorGetX(player_ship.GetPosition()) - 30, 550, 0, 0));
		bullet2.SetPosition(DirectX::XMVectorSet(DirectX::XMVectorGetX(player_ship.GetPosition()) + 30, 550, 0, 0));

		bullet1.SetTransformation(DirectX::XMMatrixRotationZ(player_ship.GetRotation()));
		bullet2.SetTransformation(DirectX::XMMatrixRotationZ(player_ship.GetRotation()));

		auto velocity = DirectX::XMVectorScale(player_ship.GetDirection(), 5.0f);

		bullets.push({ velocity, bullet1 });
		bullets.push({ velocity, bullet2 });
	}
	void FireEnemyBullet()
	{
		auto bullet1 = enemy_bullet;
		auto bullet2 = enemy_bullet;

		bullet1.SetPosition(DirectX::XMVectorSet(DirectX::XMVectorGetX(enemy_ship.GetPosition()) - 30, 280, 0, 0));
		bullet2.SetPosition(DirectX::XMVectorSet(DirectX::XMVectorGetX(enemy_ship.GetPosition()) + 30, 280, 0, 0));

		bullet1.SetTransformation(DirectX::XMMatrixRotationZ(enemy_ship.GetRotation() - DirectX::XM_PI));
		bullet2.SetTransformation(DirectX::XMMatrixRotationZ(enemy_ship.GetRotation() - DirectX::XM_PI));
		
		auto velocity = DirectX::XMVectorScale(enemy_ship.GetDirection(), -5.0f);

		enemy_bullets.push({ velocity, bullet1 });
		enemy_bullets.push({ velocity, bullet2 });
	}
};
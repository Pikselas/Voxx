#pragma once
#include <stack>
#include <queue>
#include <random>
#include "Engine/CoreEngine.h"

#include "SpaceShip.h"
#include "GameEvents.h"
#include "LaserBullet.h"
#include "TargetedMissile.h"
#include "ExplosionEffect.h"

template<typename T>
concept SkillEquipmentT = std::is_base_of_v<SkillEquipment,T>;

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
	ImageSprite path_comp;
	ImageSprite laser_bullet;
private:
	SpaceShip player_ship;
	SpaceShip enemy_ship;
private:
	ExplosionEffect default_explosion_effect;
private:
	std::deque<path_component> path_components;
private:
	std::queue<std::unique_ptr<Projectile>> player_projectiles;
	std::queue<std::unique_ptr<Projectile>> enemy_projectiles;
private:
	std::queue<std::unique_ptr<EventHolder>> internal_events;
private:
	std::queue<std::shared_ptr<ParticleEffect>> particle_effects;
public:
	constexpr static unsigned int ENEMY_Y = 250;
	constexpr static unsigned int PLAYER_Y = 580;
public:
	Scene(CoreEngine& engine) 
		: 
	engine(engine), 
	player_ship(engine, SpaceShip::Type::Ally),
	enemy_ship(engine, SpaceShip::Type::Enemy),
	default_explosion_effect(engine)
	{
		player_ship.MoveTo(509, PLAYER_Y);
		enemy_ship.MoveTo(509, ENEMY_Y);
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

		// for collision in player's ship from enemy projectiles
		size_ = player_projectiles.size();
		while (size_-- != 0)
		{
			auto projectile = std::move(player_projectiles.front());
			player_projectiles.pop();

			projectile->UpdatePosition();
			projectile->Draw(engine);

			auto pos = projectile->GetPosition();

			bool OutOfWindow = DirectX::XMVectorGetX(DirectX::XMVectorLess(pos, DirectX::XMVectorSet(0, 0, 0, 0)));
			OutOfWindow |= (bool)DirectX::XMVectorGetX(DirectX::XMVectorGreater(pos, DirectX::XMVectorSet(1018, 700, 0, 0)));

			auto [width, height] = projectile->GetSideLength();

			auto Colliding = IsColliding(pos, width/2, height/2, enemy_ship.GetPosition(), enemy_ship.GetWidth() / 2, enemy_ship.GetHeight() / 2);
			
			if (Colliding)
			{
				enemy_ship.AddHealth(-5);
				auto pos = projectile->GetPosition();
				internal_events.emplace(std::make_unique<Event<CollisionEvent>>(CreateGameEvent(CollisionEvent{ (unsigned int) DirectX::XMVectorGetX(pos) ,(unsigned int) DirectX::XMVectorGetY(pos)})));
				continue;
			}

			if (OutOfWindow)
			{
				continue;
			}

			player_projectiles.emplace(std::move(projectile));

		}

		//for collsion in enemy ship
		size_ = enemy_projectiles.size();
		while (size_-- != 0)
		{
			auto projectile = std::move(enemy_projectiles.front());
			enemy_projectiles.pop();

			projectile->UpdatePosition();
			projectile->Draw(engine);

			auto pos = projectile->GetPosition();

			bool OutOfWindow = DirectX::XMVectorGetX(DirectX::XMVectorLess(pos, DirectX::XMVectorSet(0, 0, 0, 0)));
			OutOfWindow |= (bool)DirectX::XMVectorGetX(DirectX::XMVectorGreater(pos, DirectX::XMVectorSet(1018, 700, 0, 0)));

			auto [width, height] = projectile->GetSideLength();

			auto Colliding = IsColliding(pos, width/2, height/2, player_ship.GetPosition(), player_ship.GetWidth() / 2, player_ship.GetHeight() / 2);

			if (Colliding)
			{
				
				player_ship.AddHealth(-5);
				continue;
			}

			if (OutOfWindow)
			{
				continue;
			}
			
			enemy_projectiles.emplace(std::move(projectile));
		}

		while (!internal_events.empty())
		{
			auto event_data = internal_events.front().get();
			auto deafult_action = player_ship.Skill != nullptr ? player_ship.Skill->ApplySkill(*this, player_ship, event_data) : DefaultEventAction::Enable;

			if (deafult_action == DefaultEventAction::Enable)
			{
				switch (event_data->type)
				{
					case EventHolder::Type::DisableSkill:
						player_ship.Skill.reset();
					break;
					case EventHolder::Type::ProjectileCollision:
					{
						auto pos = GetEventData<CollisionEvent>(event_data);
						default_explosion_effect.SetLocation(DirectX::XMVectorSet(pos.x, pos.y, 0, 1));
						particle_effects.emplace(std::make_shared<ExplosionEffect>(default_explosion_effect));
					}
					break;
				}
			}

			internal_events.pop();
		}

		enemy_ship.Draw(engine);
		player_ship.Draw(engine);

		auto _size = particle_effects.size();
		while (_size--)
		{
			auto effect = std::move(particle_effects.front());
			particle_effects.pop();
			effect->ApplyEffect(engine);
			if (!effect->IsCompleted())
				particle_effects.emplace(std::move(effect));
		}

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
	void SetLaserBullet(ImageSprite bullet)
	{
		laser_bullet = bullet;
	}
	void SetEnemyShipPosition(int x)
	{
		enemy_ship.MoveTo(x, 250);
		enemy_ship.RotateTowards(player_ship.GetPosition());
		player_ship.RotateTowards(enemy_ship.GetPosition());
	}
public:
	template<SkillEquipmentT SkillType , typename... ParamsT>
	void SetSkill(ParamsT... params)
	{
		if (player_ship.Skill == nullptr)
		{
			player_ship.Skill = std::make_unique<SkillType>(params...);
			internal_events.emplace(std::make_unique<EventHolder>(EventHolder::Type::ActivateSkill));
		}
	}
	template<SkillEquipmentT SkillType, typename... ParamsT>
	void SetEnemySkill(ParamsT... params)
	{
		enemy_ship.Skill = std::make_unique<SkillType>(params...);
	}
public:
	void AddEffect(std::shared_ptr<ParticleEffect> effect)
	{
		particle_effects.push(effect);
	}
public:
	void RemoveSkill()
	{
		internal_events.emplace(std::make_unique<EventHolder>(EventHolder::Type::DisableSkill));
	}
	void RemoveEnemySkill()
	{
		enemy_ship.Skill = nullptr;
	}
public:
	void Reset()
	{
		player_projectiles = decltype(player_projectiles){};
		enemy_projectiles = decltype(enemy_projectiles){};
		particle_effects = decltype(particle_effects){};
		internal_events = decltype(internal_events){};

		player_ship.Reset();
		enemy_ship.Reset();
	}
public:
	void SpaceShipController(CustomWindow& window)
	{
		player_ship.MoveTo(window.mouse.GetX(), PLAYER_Y);
		player_ship.RotateTowards(enemy_ship.GetPosition());
		enemy_ship.RotateTowards(player_ship.GetPosition());
		internal_events.emplace(std::make_unique<Event<MoveEvent>>(CreateGameEvent(MoveEvent{ (unsigned int) window.mouse.GetX()})));
	}
	void FireBullet()
	{
		player_projectiles.emplace
		( 
			std::make_unique<LaserBullet>
			(
				laser_bullet, 
				DirectX::XMVectorSet(DirectX::XMVectorGetX(player_ship.GetPosition()) - 30, 550, 0, 0),
				DirectX::XMVectorScale(player_ship.GetDirection(), 5.0f)
			)
		);
	}
	void FireEnemyBullet()
	{
		/*auto bullet1 = enemy_bullet;
		auto bullet2 = enemy_bullet;

		bullet1.SetPosition(DirectX::XMVectorSet(DirectX::XMVectorGetX(enemy_ship.GetPosition()) - 30, 280, 0, 0));
		bullet2.SetPosition(DirectX::XMVectorSet(DirectX::XMVectorGetX(enemy_ship.GetPosition()) + 30, 280, 0, 0));

		bullet1.SetTransformation(DirectX::XMMatrixRotationZ(enemy_ship.GetRotation() - DirectX::XM_PI));
		bullet2.SetTransformation(DirectX::XMMatrixRotationZ(enemy_ship.GetRotation() - DirectX::XM_PI));
		
		auto velocity = DirectX::XMVectorScale(enemy_ship.GetDirection(), -5.0f);

		enemy_bullets.push({ velocity, bullet1 });
		enemy_bullets.push({ velocity, bullet2 });*/

		enemy_projectiles.emplace
		(
			std::make_unique<LaserBullet>
			(
				laser_bullet,
				DirectX::XMVectorSet(DirectX::XMVectorGetX(enemy_ship.GetPosition()) - 30, 280, 0, 0),
				DirectX::XMVectorScale(enemy_ship.GetDirection(), 5.0f)
			)
		);

	}

	void FireTargetedMissile()
	{
		DirectX::XMVECTOR control_point;
		control_point = DirectX::XMVectorSet(std::random_device{}() % 1018, std::random_device{}() % 700, 0, 1);
		player_projectiles.emplace(std::make_unique<TargetedMissile>(laser_bullet , player_ship.GetPosition() , control_point , enemy_ship));
	}
};
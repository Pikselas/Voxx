#pragma once
#include "Scene.h"
#include "SkillEquipment.h"
#include "HealShieldEffect.h"
#include "HealExplosionEffect.h"

class HealShield : public SkillEquipment
{
private:
	HealExplosionEffect collision_effect;
	std::shared_ptr<ParticleEffect> skill_effect;
public:
	HealShield(CoreEngine& engine) 
		: 
	collision_effect(engine),
	skill_effect(std::make_shared<HealShieldEffect>(engine))
	{}
public:
	DefaultEventAction ApplySkill(Scene& scene , SpaceShip& ship, const EventHolder* const event_data) override
	{
		auto default_event_action = DefaultEventAction::Enable;
		switch (GetEventType(event_data))
		{
			case EventHolder::Type::ActivateSkill:
			{
				skill_effect->SetLocation(ship.GetPosition());
				scene.AddEffect(skill_effect);
			}
			break;
			case EventHolder::Type::Move:
			{
				skill_effect->SetLocation(ship.GetPosition());
			}
			break;
			case EventHolder::Type::ProjectileCollision:
			{
				auto data = GetEventData<CollisionEvent>(event_data);
				collision_effect.SetLocation(DirectX::XMVectorSet(data.x, data.y, 0, 1));
				scene.AddEffect(std::make_shared<HealExplosionEffect>(collision_effect));
				default_event_action = DefaultEventAction::Disable;
			}
			break;
			case EventHolder::Type::DisableSkill:
			{
				skill_effect->Cancel();
			}
			break;
		}
		return default_event_action;
	}
};
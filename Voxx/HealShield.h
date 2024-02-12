#pragma once
#include "Scene.h"
#include "SkillEquipment.h"
#include "HealShieldEffect.h"
#include "HealExplosionEffect.h"

class HealShield : public SkillEquipment
{
private:
	std::shared_ptr<ParticleEffect> skill_effect;
	//std::shared_ptr<ParticleEffect> collison_effect;
public:
	HealShield(CoreEngine& engine) 
		: 
	skill_effect(std::make_shared<HealShieldEffect>(engine))
	//collison_effect(std::make_shared<HealExplosionEffect>(engine))
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
			case EventHolder::Type::DisableSkill:
			{
				skill_effect->Cancel();
			}
			break;
		}
		return default_event_action;
	}
};
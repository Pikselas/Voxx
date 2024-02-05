#pragma once
#include "SpaceShip.h"
#include "SkillEquipment.h"
#include "ParticleEffect.h"

class HealShield : public SkillEquipment
{
private:
	std::unique_ptr<ParticleEffect> skill_effect;
public:
	HealShield(const ParticleEffect& effect) : skill_effect(std::make_unique<ParticleEffect>(effect))
	{}
public:
	void ApplySkill(SpaceShip& ship , RenderCommandEngine& engine) override
	{
		if (ship.GetSkillPoint() > 1)
		{
			//ship.AddSkillPoint(-1);
			ship.AddHealth(5);
			skill_effect->SetLocation(ship.GetPosition());
			skill_effect->ApplyEffect(engine);
		}
		else
		{
			skill_effect->Cancel();
		}
	}
};
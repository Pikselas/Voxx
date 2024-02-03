#pragma once
#include "SkillEquipment.h"
#include "SpaceShip.h"

class HealCircle : public SkillEquipment
{
private:
	AnimatedSprite heal_effect;
public:
	HealCircle(CoreEngine& engine)
	{
		std::vector<Image> sprite_data;
		sprite_data.reserve(14);

		for (int i{}; i <= 13; ++i)
		{
			sprite_data.emplace_back(R"(C:\Users\Aritra Maji\Downloads\guard_effect\green_shield_.)" + std::to_string(i) + ".png");
		}

		heal_effect = engine.CreateSprite(sprite_data, std::chrono::milliseconds(250));
	}
public:
	void ApplySkill(SpaceShip& ship , RenderCommandEngine& engine) override
	{
		if (ship.GetSkillPoint() > 1)
		{
			ship.AddSkillPoint(-1);
			ship.AddHealth(5);
			heal_effect.SetPosition(ship.GetPosition());
			heal_effect.Draw(engine);
		}
	}
};
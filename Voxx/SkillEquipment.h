#pragma once
#include "GameEvents.h"

class SkillEquipment
{
public:
	virtual DefaultEventAction ApplySkill(class Scene& scene, class SpaceShip& ship, const class EventHolder* const event_data) = 0;
	virtual ~SkillEquipment() = default;
};
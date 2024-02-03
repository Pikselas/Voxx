#pragma once

class SkillEquipment
{
public:
	virtual void ApplySkill(class SpaceShip& , class RenderCommandEngine&) = 0;
};
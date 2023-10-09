#pragma once
#include "Engine/CoreEngine.h"

class Goblin : public Sprite
{
private:
	ImageSprite main_sprite;
	ImageSprite weapon_sprite;
public:
	Goblin(CoreEngine& engine) 
		: 
	main_sprite(engine.CreateSprite(Image("C:/Users/Aritra Maji/Downloads/OIG-removebg-preview.png"))), 
	weapon_sprite(engine.CreateSprite(Image("C:/Users/Aritra Maji/Downloads/oig_dk.png")))
	{
		constexpr int half_width = (145 + 243) / 2;
		constexpr int main_image_shift_by = -(half_width - (145 / 2));
		constexpr int weapon_image_shift_by = main_image_shift_by + 145;

		main_sprite.SetPosition(DirectX::XMVectorSet(main_image_shift_by , 0.0f , 0.0f , 1));
		weapon_sprite.SetPosition(DirectX::XMVectorSet(weapon_image_shift_by , 30.0f , 0.0f , 1));
	}
public:
	void Draw(Engine& engine) const override
	{
		engine.SetComponent(weapon_sprite.GetTransformation() * GetTransformation());
		weapon_sprite.DrawNonTransformed(engine);
		engine.SetComponent(main_sprite.GetTransformation() * GetTransformation());
		main_sprite.DrawNonTransformed(engine);
	}
protected:
	void DrawNonTransformed(Engine& engine) const override
	{
		weapon_sprite.Draw(engine);
		main_sprite.Draw(engine);
	}
};
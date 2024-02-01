#pragma once
#include "Projectile.h"

class LaserBullet : public Projectile
{
public:
	LaserBullet(ImageSprite sprite , DirectX::XMVECTOR position , DirectX::XMVECTOR velocity) 
		: 
	Projectile(sprite , velocity , position)
	{
		auto direction = std::atan2(DirectX::XMVectorGetY(velocity), DirectX::XMVectorGetX(velocity));
		projectile.SetTransformation(DirectX::XMMatrixRotationZ(direction));
	}
public:
	void UpdatePosition() override
	{
		projectile.SetPosition(DirectX::XMVectorAdd(projectile.GetPosition(), control_parameter));
	}
};
#pragma once
#include "Projectile.h"

class LaserBullet : public Projectile
{
public:
	LaserBullet(CoreEngine& engine , DirectX::XMVECTOR position , DirectX::XMVECTOR velocity) 
		: 
	Projectile(engine , "media/bullet.png" , velocity , position)
	{
		auto direction = std::atan2(DirectX::XMVectorGetY(velocity), DirectX::XMVectorGetX(velocity));
		projectile.SetTransformation(DirectX::XMMatrixRotationZ(direction));
	}
public:
	void Update() override
	{
		projectile.SetPosition(DirectX::XMVectorAdd(projectile.GetPosition(), control_parameter));
	}
};
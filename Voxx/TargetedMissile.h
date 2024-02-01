#pragma once
#include "Projectile.h"
#include "SpaceShip.h"

class TargetedMissile : public Projectile 
{
private:
	float lerp_dist;
private:
	const SpaceShip& target;
public:
	TargetedMissile(ImageSprite sprite, DirectX::XMVECTOR position, DirectX::XMVECTOR control_point , const SpaceShip& target)
		:
		Projectile(sprite, control_point , position) , lerp_dist(0.0f), target(target)
	{}
private:
	static std::vector<DirectX::XMVECTOR> getCurvePoint(float t, std::vector<DirectX::XMVECTOR> control_points)
	{
		if (control_points.size() > 1)
		{
			std::vector<DirectX::XMVECTOR> new_points;
			for (int i = 0; i < control_points.size() - 1; ++i)
			{
				new_points.emplace_back(DirectX::XMVectorLerp(control_points[i], control_points[i + 1], t));
			}
			return getCurvePoint(t, new_points);
		}
		return control_points;
	}
public:
	void UpdatePosition() override
	{
		auto new_pos = getCurvePoint(lerp_dist, {projectile.GetPosition() , control_parameter , target.GetPosition()})[0];
		auto dir = DirectX::XMVectorSubtract(new_pos, projectile.GetPosition());
		auto rotation_angle = std::atan2(DirectX::XMVectorGetY(dir) , DirectX::XMVectorGetX(dir));
		projectile.SetTransformation(DirectX::XMMatrixRotationZ(rotation_angle));
		projectile.SetPosition(new_pos);
		lerp_dist += 0.005;
	}
};
#pragma once
#include "ParticleEffect.h"

class HealExplosionEffect : public ParticleEffect
{
public:
	HealExplosionEffect(CoreEngine& engine) : ParticleEffect(engine , "media/effects/green_explosion.{}.png" , 6 , std::chrono::milliseconds(500) ,1)
	{
		effect_animation.SetTransformation(DirectX::XMMatrixScaling( 0.3f , 0.3f , 1.0f));
	}
};
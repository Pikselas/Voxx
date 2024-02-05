#pragma once
#include "ParticleEffect.h"

class ExplosionEffect : public ParticleEffect
{
public:
	ExplosionEffect(CoreEngine& engine)
		:
		ParticleEffect(engine, "media/effects/explosion ({}).png", 7, std::chrono::milliseconds(700) , 1)
	{
		effect_animation.SetTransformation(DirectX::XMMatrixScaling(0.3, 0.3, 1));
	}
};
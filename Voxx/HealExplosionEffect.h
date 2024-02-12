#pragma once
#include "ParticleEffect.h"

class HealExplosionEffect : public ParticleEffect
{
public:
	HealExplosionEffect(CoreEngine& engine) : ParticleEffect(engine , "media/effects/green_explode_.{}.png" , 7 , std::chrono::milliseconds(500) ,1)
	{}
};
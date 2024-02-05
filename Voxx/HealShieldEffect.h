#pragma once
#include "ParticleEffect.h"

class HealShieldEffect : public ParticleEffect
{
public:
	HealShieldEffect(CoreEngine& engine) 
		:
	ParticleEffect(engine , "media/effects/green_shield_.{}.png" , 14 , std::chrono::milliseconds(500))
	{}
};
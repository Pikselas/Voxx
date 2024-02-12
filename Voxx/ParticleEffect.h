#pragma once
#include <format>
#include "Engine/CoreEngine.h"

class ParticleEffect
{
protected:
	AnimatedSprite effect_animation;
protected:
	ParticleEffect(CoreEngine& engine, const std::string& fmt_path, unsigned int frame_count, std::chrono::milliseconds duration, std::optional<unsigned int> repeat_count = std::nullopt)
	{
		std::vector<Image> frames;
		frames.reserve(frame_count);

		for (int i{}; i < frame_count; ++i)
		{
			frames.emplace_back(std::vformat(fmt_path, std::make_format_args(i)));
		}

		effect_animation = engine.CreateSprite(frames , duration , repeat_count);
	}
public:
	void ApplyEffect(RenderCommandEngine& engine)
	{
		effect_animation.Draw(engine);
	}
	void SetLocation(DirectX::XMVECTOR position)
	{
		effect_animation.SetPosition(position);
	}
	void Cancel()
	{
		effect_animation.SetRepeatCount(0);
	}
	bool IsCompleted() const
	{
		return effect_animation.IsStopped();
	}
	virtual ~ParticleEffect() = default;
};
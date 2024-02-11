#pragma once
#include <map>
#include <queue>
#include <functional>

struct MoveEvent
{
	unsigned int x;
};

struct FireEvent
{};

struct ReadyEvent
{};

struct CollisionEvent
{
	unsigned int x, y;
};

template<typename T>
concept SkillType = true; //std::is_base_of_v<class SkillEquipment, T>;

struct SkillEvent
{
	size_t type_hash;
	SkillEvent(size_t type_hash) : type_hash(type_hash){}
};

template<SkillType Skill>
struct ActivateSkillEvent : public SkillEvent
{
	using Type = Skill;
	ActivateSkillEvent() : SkillEvent(typeid(Skill).hash_code()) {}
};

struct DisableSkillEvent
{};

struct EventHolder
{
	enum class Type
	{
		Move, 
		Fire,
		Ready,
		ActivateSkill,
		DisableSkill,
		ProjectileCollision
	};
	Type type;

	EventHolder() = default;
	EventHolder(Type type) : type(type) {}

};

enum class DefaultEventAction
{
	Enable,
	Disable
};

template<typename EventType>
struct Event : public EventHolder
{
	EventType event_data;
};

template<typename EventType>
Event<EventType> CreateGameEvent(EventType event_data)
{
	Event<EventType> ev;
	if constexpr (std::is_same_v<EventType, MoveEvent>)
	{
		ev.type = EventHolder::Type::Move;
	}
	else if constexpr (std::is_same_v<EventType, FireEvent>)
	{
		ev.type = EventHolder::Type::Fire;
	}
	else if constexpr (std::is_same_v<EventType, ReadyEvent>)
	{
		ev.type = EventHolder::Type::Ready;
	}
	else if constexpr (std::is_same_v<EventType, DisableSkillEvent>)
	{
		ev.type = EventHolder::Type::DisableSkill;
	}
	else if constexpr (std::is_same_v<EventType, CollisionEvent>)
	{
		ev.type = EventHolder::Type::ProjectileCollision;
	}
	else if constexpr (std::is_same_v<EventType, ActivateSkillEvent<typename EventType::Type>>)
	{
		ev.type = EventHolder::Type::ActivateSkill;
	}
	ev.event_data = event_data;
	return ev;
}

EventHolder::Type GetEventType(const EventHolder* const event_data)
{
	return reinterpret_cast<const EventHolder*>(event_data)->type;
}

template<typename EventType>
EventType GetEventData(const EventHolder* const event_data)
{
	return reinterpret_cast<const Event<EventType>*>(event_data)->event_data;
}
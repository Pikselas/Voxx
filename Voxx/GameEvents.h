#pragma once
struct MoveEvent
{
	unsigned int x;
};

struct FireEvent
{};

struct ReadyEvent
{};

template<typename T>
concept SkillType = std::is_base_of_v<class SkillEquipment, T>;

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

class EventHolder
{
public:
	enum class Type
	{
		Move, 
		Fire,
		Ready,
		ActivateSkill,
		DisableSkill
	};
	Type type;
};

template<typename EventType>
class Event : public EventHolder
{
public:
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
	else if constexpr (std::is_same_v<EventType, ActivateSkillEvent<typename EventType::Type>>)
	{
		ev.type = EventHolder::Type::ActivateSkill;
	}
	ev.event_data = event_data;
	return ev;
}

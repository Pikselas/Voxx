#pragma once
struct MoveEvent
{
	unsigned int x;
};

struct FireEvent
{};

struct ReadyEvent
{};

class EventHolder
{
public:
	enum class Type
	{
		Move, 
		Fire,
		Ready,
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
	ev.event_data = event_data;
	return ev;
}

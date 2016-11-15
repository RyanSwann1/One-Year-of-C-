#pragma once

#include "StateType.h"
#include "EventDetails.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <string>
#include <SFML\Window.hpp>

struct EventInfo
{
	EventInfo(const int keyCode)
		: m_keyCode(keyCode) {}

	int m_keyCode;
};

struct Binding
{
	Binding(const std::string& name)
		: m_name(name),
		m_count(0),
		m_eventDetails(name)
	{}

	void addEvent(const EventInfo info, const EventType type)
	{
		m_events.emplace_back(info, type);
	}

	std::vector<std::pair<EventInfo, EventType>> m_events;
	std::string m_name;
	EventDetails m_eventDetails;
	int m_count;
};

using CallbackContainer = std::pair<StateType, std::function<void(const EventDetails&)>>;
using std::placeholders::_1;
struct SharedContext;
class EventManager
{
public:
	EventManager(SharedContext& sharedContext);
	~EventManager();

	//template <class T>
	//void addCallback(const std::string& name, const StateType stateType, void(T::*fp)(const EventDetails*), T* instance)
	//{
	//	if (m_callBacks.find(name) == m_callBacks.cend())
	//	{
	//		auto callBackContainer = std::make_pair(stateType, std::bind(fp, instance, _1));
	//		m_callBacks.emplace(name, callBackContainer);
	//	}
	//}

	template <class T>
	void addCallback(const std::string& name, const StateType stateType, void(T::*fp)(const EventDetails&), T* instance)
	{
		if (m_callBacks.find(name) == m_callBacks.cend())
		{
			auto callBackContainer = std::make_pair(stateType, std::bind(fp, instance, _1));
			m_callBacks.emplace(std::make_pair(name, callBackContainer));
		}
	}
	void removeCallback(const std::string& name);

	void handleEvent(sf::Event polledEvent);
	void update();

private:
	std::unordered_map<std::string, CallbackContainer> m_callBacks;
	std::vector<Binding> m_bindings;
	SharedContext* m_sharedContext;
	void loadInBindings(const std::string& fileName);
	void purgeCallbacks();
};
#pragma once

#include "EventType.h"

struct EventDetails
{
	EventDetails(const std::string& name)
		: m_keyCode(-1),
		m_name(name) {}

	std::string m_name;
	EventType m_eventType;
	int m_keyCode;
};
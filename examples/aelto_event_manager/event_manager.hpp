#pragma once

#include <vector>

#include <chestnut/fsm/statemachine.hpp>
using namespace chestnut::fsm;

#include "doubles.hpp"
#include "event_listener.hpp"
class RER_EventsManagerStateStarting;


class RER_EventsManager : public Statemachine<>
{
public:
	std::vector<RER_EventsListener> listeners;

	void addListener( RER_EventsListener listener ) {
		this->listeners.push_back(listener);
	}

	CRandomEncounters master;

	RER_EventsManager(CRandomEncounters master) {
		this->master = master;

		this->addListener(RER_EventsListener());
		this->addListener(RER_EventsListener());
		this->addListener(RER_EventsListener());
		this->addListener(RER_EventsListener());
		this->addListener(RER_EventsListener());
	}

	float internal_cooldown;

	float delay;

	float chance_scale;

	void start() {
		LogChannel("modRandomEncounters", "RER_EventsManager - start()");

		this->delay = this->master.settings.event_system_interval;

		if(this->delay > 0) {
			this->gotoState<RER_EventsManagerStateStarting>();
		}
	}
};
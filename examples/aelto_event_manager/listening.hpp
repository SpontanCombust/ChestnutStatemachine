#pragma once

#include <chestnut/fsm/state.hpp>

#include "event_manager.hpp"
class RER_EventsManagerStateWaiting;


class RER_EventsManagerStateListeningForEvents : public State<RER_EventsManager>
{
public:
    void onEnterState( StateTransition transition ) override {
		super::onEnterState(transition);
		LogChannel("modRandomEncounters", "RER_EventsManager - State ListeningForEvents");

		this->ListeningForEvents_main();
    }

    void ListeningForEvents_main() {
		int i;
		RER_EventsListener listener;
		bool was_spawn_already_triggered;
		bool spawn_asked;

		was_spawn_already_triggered = false;

		LogChannel("modRandomEncounters", "RER_EventsManager - State ListeningForEvents - listening started");

		if(!getParent().master.settings.is_enabled) {
			getParent().gotoState<RER_EventsManagerStateWaiting>();
		}

		for(i = 0; i < getParent().listeners.size(); i += 1) {
			listener = getParent().listeners[i];

			if(!listener.is_ready) {
				listener.onReady(getParent());
			}

			if(!listener.active) {
				continue;
			}

			was_spawn_already_triggered = listener
				.onInterval(was_spawn_already_triggered, getParent().master, getParent().delay, getParent().chance_scale) || was_spawn_already_triggered;
		}

		LogChannel("modRandomEncounters", "RER_EventsManager - State ListeningForEvents - listening finished");

		getParent().gotoState<RER_EventsManagerStateWaiting>();
    }


private:
	typedef BaseStateType super;
};
#pragma once

#include <chestnut/fsm/state.hpp>

#include "event_manager.hpp"
class RER_EventsManagerStateListeningForEvents;


class RER_EventsManagerStateWaiting : public State<RER_EventsManager>
{
public:
    void onEnterState( StateTransition transition ) override {
		super::onEnterState(transition);
		LogChannel("modRandomEncounters", "RER_EventsManager - State Waiting");

		this->Waiting_main();
    }

    void Waiting_main() {
		LogChannel("modRandomEncounters", "RER_EventsManager - Waiting_main()");

		Sleep(getParent().delay);

		getParent().gotoState<RER_EventsManagerStateListeningForEvents>();
    }


private:
	typedef BaseStateType super;
};
#pragma once

#include <chestnut/fsm/state.hpp>

#include "event_manager.hpp"
class RER_EventsManagerStateWaiting;


class RER_EventsManagerStateStarting : public State<RER_EventsManager>
{
public:
    void onEnterState(StateTransition transition) override {
        super::onEnterState(transition);
        LogChannel("modRandomEncounters", "RER_EventsManager - State Starting");

        this->Starting_main();
    }

    void Starting_main() {
        CInGameConfigWrapper inGameConfigWrapper;
        RER_EventsListener listener;
        int i;

        for(i = 0; i < getParent().listeners.size(); i += 1) {
            listener = getParent().listeners[i];

			if(!listener.is_ready) {
				listener.onReady(getParent());
			}

			listener.loadSettings();
        }

		inGameConfigWrapper = theGame.GetInGameConfigWrapper();

		getParent().internal_cooldown = std::stof( 
			inGameConfigWrapper
			.GetVarValue("RERevents", "eventSystemICD")
		);

		getParent().chance_scale = getParent().delay / getParent().internal_cooldown;

		LogChannel("modRandomEncounters", "RER_EventsManager - chance_scale = " << getParent().chance_scale << ", delay =" << getParent().delay);

		getParent().gotoState<RER_EventsManagerStateWaiting>();
    }


private:
    typedef BaseStateType super;
};
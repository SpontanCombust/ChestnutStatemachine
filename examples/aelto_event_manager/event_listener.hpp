#pragma once

#include "doubles.hpp"
class RER_EventsManager;


class RER_EventsListener {
public:
    bool active;

    bool is_ready = false;

    virtual void onReady(RER_EventsManager& manager) {
        this->active = true;
        this->loadSettings();
    }

    virtual bool onInterval(bool was_spawn_already_triggered, CRandomEncounters master, float delta, float chance_scale) {
        return was_spawn_already_triggered;
    }

    virtual void loadSettings() {}
};
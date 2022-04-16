/**
 * @example aelto_event_manager.cpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Example of statemachine code ported from WitcherScript itself
 * @details 
 * Code for this example was written based on the code of the mod for The Witcher 3 - Random Encounters Reworked by Thibault Hottou aka Aelto
 * https://github.com/Aelto/tw3-random-encounters-reworked
 * @version 3.0.0
 * @date 2022-01-31
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 * @include aelto_event_manager/doubles.hpp
 * doubles.hpp
 * @include aelto_event_manager/event_listener.hpp
 * event_listener.hpp
 * @include aelto_event_manager/event_manager.hpp
 * event_manager.hpp
 * @include aelto_event_manager/starting.hpp
 * starting.hpp
 * @include aelto_event_manager/waiting.hpp
 * waiting.hpp
 * @include aelto_event_manager/listening.hpp
 * listening.hpp
 */

#include "aelto_event_manager/event_listener.hpp"
#include "aelto_event_manager/event_manager.hpp"
#include "aelto_event_manager/starting.hpp"
#include "aelto_event_manager/waiting.hpp"
#include "aelto_event_manager/listening.hpp"

int main(int argc, char const *argv[])
{
    CRandomEncounters master;
    RER_EventsManager manager(master);

    // this goes on a never ending loop
    manager.start();

    return 0;
}

/* CONSOLE OUTPUT
[modRandomEncounters] RER_EventsManager - start()
[modRandomEncounters] RER_EventsManager - State Starting
[modRandomEncounters] RER_EventsManager - chance_scale = 500, delay =1000
[modRandomEncounters] RER_EventsManager - State Waiting
[modRandomEncounters] RER_EventsManager - Waiting_main()
[modRandomEncounters] RER_EventsManager - State ListeningForEvents
[modRandomEncounters] RER_EventsManager - State ListeningForEvents - listening started
[modRandomEncounters] RER_EventsManager - State ListeningForEvents - listening finished
[modRandomEncounters] RER_EventsManager - State Waiting
[modRandomEncounters] RER_EventsManager - Waiting_main()
[modRandomEncounters] RER_EventsManager - State ListeningForEvents
[modRandomEncounters] RER_EventsManager - State ListeningForEvents - listening started
[modRandomEncounters] RER_EventsManager - State ListeningForEvents - listening finished
[modRandomEncounters] RER_EventsManager - State Waiting
[modRandomEncounters] RER_EventsManager - Waiting_main()
[modRandomEncounters] RER_EventsManager - State ListeningForEvents
[modRandomEncounters] RER_EventsManager - State ListeningForEvents - listening started
[modRandomEncounters] RER_EventsManager - State ListeningForEvents - listening finished
[modRandomEncounters] RER_EventsManager - State Waiting
[modRandomEncounters] RER_EventsManager - Waiting_main()
[modRandomEncounters] RER_EventsManager - State ListeningForEvents
[modRandomEncounters] RER_EventsManager - State ListeningForEvents - listening started
[modRandomEncounters] RER_EventsManager - State ListeningForEvents - listening finished
[modRandomEncounters] RER_EventsManager - State Waiting
[modRandomEncounters] RER_EventsManager - Waiting_main()
...
...
*/
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
#include "../include/chestnut/statemachine/state.hpp"
#include "../include/chestnut/statemachine/statemachine.hpp"

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

using namespace chestnut::statemachine;




// 1. Define the state name type - it can be an enum like shown below
enum EDoorStateName
{
    DOOR_OPEN,
    DOOR_OPENING,
    DOOR_CLOSED,
    DOOR_CLOSING
};

const char *doorStateNameToString( EDoorStateName name )
{
    switch( name )
    {
    case DOOR_OPEN: return "Open";
    case DOOR_OPENING: return "Opening";
    case DOOR_CLOSED: return "Closed";
    case DOOR_CLOSING: return "Closing";
    default: return "";
    }
}




// 2. Forward declare statemachine type and define the state interface
class CDoorStatemachine;

class IDoorState : public IState<EDoorStateName, CDoorStatemachine>
{
public:
    typedef IState<EDoorStateName, CDoorStatemachine> super;

    // 2.1 You have to define the special constructor that takes in statemachine pointer, you can pass the pointer to parent class constructor
    // At the moment this library doesn't support custom state constructors
    IDoorState( CDoorStatemachine *sm ) : super( sm ) {}

    virtual bool tryOpen() = 0;
    virtual bool tryClose() = 0;
};




// 3. Define your statemachine type
class CDoorStatemachine : public IStatemachine<EDoorStateName, IDoorState>
{
public:
    // utility parent class typedef
    typedef IStatemachine<EDoorStateName, IDoorState> super;

    // You can make your statemachine able to be used across threads in an async manner
    // You have to be very careful with this however, like with any other scenario that involves race conditions
    // Statemachine is not thread safe on its own!
    mutable std::recursive_mutex doorMutex;


    // We'll override methods to account for the mutex
    IDoorState *getCurrentState() const override
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        IDoorState *state = super::getCurrentState();

        return state;
    }

    EDoorStateName getCurrentStateName() const override
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        EDoorStateName name = super::getCurrentStateName();

        return name;
    }

    void gotoState( EDoorStateName nextStateName ) override
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        super::gotoState( nextStateName );
    }

    void pushState( EDoorStateName nextStateName ) override
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        super::pushState( nextStateName );
    }

    void popState() override
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        super::popState();
    }



    bool tryOpen()
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        return super::getCurrentState()->tryOpen();
    }

    bool tryClose()
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        return getCurrentState()->tryClose();
    }

};




// 4. Define your states
//
// If you put state interface and statemachine definition in seperate headers 
// you'll want to include them both in states' headers if you want to use the parent pointer
class CDoorStateClosed : public IDoorState
{
public:
    // Again we have to define a special constructor
    CDoorStateClosed( CDoorStatemachine *sm ) : IDoorState( sm ) 
    {
        // Remember to set the name for your state!
        name = DOOR_CLOSED;
    }

    void onEnter( EDoorStateName prevState ) override
    {
        // guard in case this is the default state
        if( prevState != DOOR_CLOSED )
        {
            std::cout << "The door is now closed!\n";
        }
    }

    void onExit( EDoorStateName nextState ) override
    {
        std::cout << "The door is no longer closed!\n";
    }
    
    bool tryOpen() override
    {
        parent->gotoState( DOOR_OPENING );   
        return true;  
    }

    bool tryClose() override
    {
        std::cout << "The door is already closed!\n";
        return false;
    }
};

class CDoorStateOpening : public IDoorState
{
public:
    // Again we have to define a special constructor
    CDoorStateOpening( CDoorStatemachine *sm ) : IDoorState( sm ) 
    {
        // Remember to set the name for your state!
        name = DOOR_OPENING;
    }

    void onEnter( EDoorStateName prevState ) override
    {
        std::cout << "The door is openning...\n";

        std::thread( [this] {
            std::this_thread::sleep_for( std::chrono::seconds(2) );
            parent->gotoState( DOOR_OPEN );
        }).detach();
    }

    void onExit( EDoorStateName nextState ) override
    {
        std::cout << "The door has finished openning!\n";
    }
    
    bool tryOpen() override
    {
        std::cout << "The door is already in the process of openning!\n";
        return false;
    }

    bool tryClose() override
    {
        std::cout << "I'm sorry. I first need to finish opening.\n";
        return false;
    }
};

class CDoorStateOpen : public IDoorState
{
public:
    // Again we have to define a special constructor
    CDoorStateOpen( CDoorStatemachine *sm ) : IDoorState( sm ) 
    {
        // Remember to set the name for your state!
        name = DOOR_OPEN;
    }

    void onEnter( EDoorStateName prevState ) override
    {
        // guard in case this is the default state
        if( prevState != DOOR_OPEN )
        {
            std::cout << "The door is now open!\n";
        }
    }

    void onExit( EDoorStateName nextState ) override
    {
        std::cout << "The door is no longer open!\n";
    }
    
    bool tryOpen() override
    {
        std::cout << "The door is already open!\n";     
        return false;
    }

    bool tryClose() override
    {
        parent->gotoState( DOOR_CLOSING );
        return true;
    }
};

class CDoorStateClosing : public IDoorState
{
public:
    // Again we have to define a special constructor
    CDoorStateClosing( CDoorStatemachine *sm ) : IDoorState( sm ) 
    {
        // Remember to set the name for your state!
        name = DOOR_CLOSING;
    }

    void onEnter( EDoorStateName prevState ) override
    {
        std::cout << "The door is closing...\n";

        std::thread( [this] {
            std::this_thread::sleep_for( std::chrono::seconds(2) );
            parent->gotoState( DOOR_CLOSED );
        }).detach();
    }

    void onExit( EDoorStateName nextState ) override
    {
        std::cout << "The door has finished closing!\n";
    }
    
    bool tryOpen() override
    {   
        std::cout << "I'm sorry. I first need to finish closing.\n";
        return false;
    }

    bool tryClose() override
    {
        std::cout << "The door is already in the process of closing!\n";
        return false;
    }
};




// 5. Create a statemachine object
int main(int argc, char const *argv[])
{
    CDoorStatemachine door;
    door.setupStates< CDoorStateClosed, CDoorStateOpen, CDoorStateClosing, CDoorStateOpening >();

    auto printDoorState = [&door] {
        std::cout << "Door state: " << doorStateNameToString( door.getCurrentStateName() ) << "\n";
    };


    printDoorState();
    if( door.tryClose() )
    {
        printDoorState();
    }
    if( door.tryOpen() )
    {
        printDoorState();
        while( door.getCurrentStateName() == DOOR_OPENING )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds(100) );
        }
        printDoorState();

        if( door.tryClose() )
        {
            printDoorState();
            while( door.getCurrentStateName() == DOOR_CLOSING )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds(100) );
            }
            printDoorState();
        }
    }

    return 0;
}

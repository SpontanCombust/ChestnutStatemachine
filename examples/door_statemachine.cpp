// =============================== 0. Include the statemachine.hpp header file =================================
#include "../include/chestnut/statemachine/statemachine.hpp"

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>




// ===================== 1. Forward declare statemachine type and define the base state type =====================

using namespace chestnut; // use the chestnut root namespace name for convenience

// 1.1. Forward declare statemachine type, because we need it for the base state type
class CDoorStatemachine;

// 1.2. Create a type that will derive from IState generic class, for the template parameter give it the statemachine type from point 1.1
class IDoorState : public fsm::IState<CDoorStatemachine>
{
public:
// 1.3. (Optional) If you want to call methods on current states from the statemachine 
// and you want them to be overridable you can define some public virtual methods here
    virtual bool tryOpen() = 0;
    virtual bool tryClose() = 0;
};




// ====================================== 2. Define your statemachine type ======================================


// 2.1. Create a type that will derive from IStatemachine generic class, for the template parameter give it the base state type from point 1.2
class CDoorStatemachine : public fsm::IStatemachine<IDoorState>
{
public:
    // 2.2. (Optional) You can make your statemachine able to be used across threads in an async manner
    // The base IStatemachine type does not support multithreading, so you'll have to set up the necessary precautions yourself
    mutable std::recursive_mutex doorMutex;


    CDoorStatemachine();


    // 2.3. (Optional) If you want you can create methods in the statemachine with the same signatures that you gave in point 1.3
    // This will make them look as a sort of "state-virtual" methods, 
    // i.e. methods which behaviour depends almost solely on the current state of the statemachine

    bool tryOpen()
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        return getCurrentState()->tryOpen();
    }

    bool tryClose()
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        return getCurrentState()->tryClose();
    }
};




// ====================================== 3. Define your states ======================================

// 3.1. Be aware of the order of your includes!
//
// Calls to init(), gotoState() and pushState() require that the state type given to them in the template parameter
// is a *complete* type.
// If you need a mutual state dependency (e.g. state A goes to state B and vice versa)
// it's best to properly divide state class body and its methods' definitions.
// You can't make state transition to a forward declared state!
// If you decide to write a statemachine it'd be a lot better to keep each state (and the statemachine itself) in seperate .h and .cpp files 
//
// More on incomplete types: https://en.cppreference.com/w/cpp/language/type#Incomplete_type
class CDoorStateClosed : public IDoorState
{
public:
    // 3.2. (Optional) States can have a custom constructor or just a default one
    // You pass these custom parameters when you call init(), gotoState() or pushState()
    CDoorStateClosed( bool printOnNullState = false );

    // 3.3. (Optional) Override methods that will be called when a statemachine enter or leaves the state
    // The data about the transition (whether it was done with popState() method for example) is in StateTransition struct in the parameter
    //
    // As the statemachine requires that at least one state stays on the state stack throughout statemachine's lifetime after init()
    // you could as well create a completely empty state with no overrides!
    void onEnterState( fsm::StateTransition transition ) override;
    void onLeaveState( fsm::StateTransition transition ) override;
    
    // 3.4. (Optional) Override the virtual methods you specified in point 1.3
    bool tryOpen() override;
    bool tryClose() override;

private:
    bool printOnNullState;
};

class CDoorStateOpening : public IDoorState
{
public:
    // 3.5. You don't even need to define a constructor at all!

    void onEnterState( fsm::StateTransition transition ) override;
    void onLeaveState( fsm::StateTransition transition ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
};

class CDoorStateOpen : public IDoorState
{
public:
    void onEnterState( fsm::StateTransition transition ) override;
    void onLeaveState( fsm::StateTransition transition ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
};

class CDoorStateClosing : public IDoorState
{
public:
    void onEnterState( fsm::StateTransition transition ) override;
    void onLeaveState( fsm::StateTransition transition ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
};


CDoorStatemachine::CDoorStatemachine() 
{
    // 3.6. A statemachine has to be initialized with some entry state
    // This state will stay on the state stack throughout the lifetime of the statemachine and won't be possible to get poppped
    // If you don't have a state that could be good candidate for a state that the machine should always eventually transition back to
    // you can simply create an empty state! Just inherit from the base state type and that's it. 
    // IState does not force you to override any pure virtual methods or create any special constructors.
    //
    // Based on point 3.1 if we wanted to use a state transition method on CDoorStateClosed we have to use it AFTER its complete type body.
    // Hence why CDoorStatemachine() constructor definition is all the way here, but it's only because we're working in a single .cpp file.

    // 3.7. (Optional) We can call state transition method with custom parameters. These parameters will be forwarded to state's constructor. See point 3.2.
    // init() can be called on the statemachine object itself, but this way we can hide away its statemachine nature.
    init<CDoorStateClosed>( true );
}

CDoorStateClosed::CDoorStateClosed( bool printOnNullState )
{
    this->printOnNullState = printOnNullState;
}

void CDoorStateClosed::onEnterState( fsm::StateTransition transition )
{
    // message won't show if this is the init state
    if( transition.type != fsm::STATE_TRANSITION_INIT )
    {
        std::cout << "The door is now closed!\n";
    }
    else if( printOnNullState )
    {
        std::cout << "CDoorStateClosed is the init state!\n";
    }
}

void CDoorStateClosed::onLeaveState( fsm::StateTransition transition )
{
    // message won't show if statemachine is being deleted
    if( transition.type != fsm::STATE_TRANSITION_DESTROY )
    {
        std::cout << "The door is no longer closed!\n";
    }
    else if( printOnNullState )
    {
        std::cout << "Statemachine is being destroyed!\n";
    }
}

bool CDoorStateClosed::tryOpen()
{
    std::lock_guard< std::recursive_mutex > lock( parent->doorMutex );
    parent->pushState<CDoorStateOpening>();
    return true;  
}

bool CDoorStateClosed::tryClose()
{
    std::cout << "The door is already closed!\n";
    return false;
}



void CDoorStateOpening::onEnterState( fsm::StateTransition transition ) 
{
    std::cout << "The door is openning...\n";

    // spawn a thread that'll wait 2 seconds and then transition to next state
    std::thread( [this] {
        std::this_thread::sleep_for( std::chrono::seconds(2) );
        std::lock_guard< std::recursive_mutex > lock( parent->doorMutex );
        parent->gotoState<CDoorStateOpen>();
    }).detach();
}

void CDoorStateOpening::onLeaveState( fsm::StateTransition transition ) 
{
    std::cout << "The door has finished openning!\n";
}

bool CDoorStateOpening::tryOpen() 
{
    std::cout << "The door is already in the process of openning!\n";
    return false;
}

bool CDoorStateOpening::tryClose() 
{
    std::cout << "I'm sorry. I first need to finish opening.\n";
    return false;
}



void CDoorStateOpen::onEnterState( fsm::StateTransition transition ) 
{
    // message won't show if this is the init state
    if( transition.type != fsm::STATE_TRANSITION_INIT )
    {
        std::cout << "The door is now open!\n";
    }
}

void CDoorStateOpen::onLeaveState( fsm::StateTransition transition ) 
{
    // message won't show if statemachine is being deleted
    if( transition.type != fsm::STATE_TRANSITION_DESTROY )
    {
        std::cout << "The door is no longer open!\n";
    }
}

bool CDoorStateOpen::tryOpen() 
{
    std::cout << "The door is already open!\n";     
    return false;
}

bool CDoorStateOpen::tryClose()
{
    std::lock_guard< std::recursive_mutex > lock( parent->doorMutex );
    parent->gotoState<CDoorStateClosing>();
    return true;
}



void CDoorStateClosing::onEnterState( fsm::StateTransition transition ) 
{
    std::cout << "The door is closing...\n";

    std::thread( [this] {
        std::this_thread::sleep_for( std::chrono::seconds(2) );
        std::lock_guard< std::recursive_mutex > lock( parent->doorMutex );
        parent->popState();
    }).detach();
}

void CDoorStateClosing::onLeaveState( fsm::StateTransition transition )
{
    std::cout << "The door has finished closing!\n";
}

bool CDoorStateClosing::tryOpen() 
{   
    std::cout << "I'm sorry. I first need to finish closing.\n";
    return false;
}

bool CDoorStateClosing::tryClose()
{
    std::cout << "The door is already in the process of closing!\n";
    return false;
}





const char *doorStateTypeToString( std::type_index type )
{
    if( type == typeid(CDoorStateOpen ) )
        return "Open";
    if( type == typeid(CDoorStateOpening ) )
        return "Opening";
    if( type == typeid(CDoorStateClosed ) )
        return "Closed";
    if( type == typeid(CDoorStateClosing ) )
        return "Closing";
    return "";
}

// ===================== 4. Create and setup your statemachine object =====================

int main(int argc, char const *argv[])
{

    CDoorStatemachine door;

    auto printDoorState = [&door] {
        std::cout << "Door state: " << doorStateTypeToString( door.getCurrentStateType() ) << "; state stack size: " << door.getStateStackSize() << "\n";
    };

    printDoorState();
    if( door.tryClose() )
    {
        printDoorState();
    }
    if( door.tryOpen() )
    {
        printDoorState();

        while( door.isCurrentlyInState<CDoorStateOpening>() )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds(100) ); // we'll keep waiting small intervals until the door fully opens

            std::lock_guard< std::recursive_mutex > lock( door.doorMutex );
        }

        printDoorState();

        if( door.tryClose() )
        {
            printDoorState();

            while( door.isCurrentlyInState<CDoorStateClosing>() )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds(100) );

                std::lock_guard< std::recursive_mutex > lock( door.doorMutex );
            }

            printDoorState();
        }
    }

    return 0;
}

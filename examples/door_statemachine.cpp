/**
 * @example door_statemachine.cpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief A source file with door statemachine example
 * @version 3.0.0
 * @date 2022-01-20
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */

#include <iostream>
#include <mutex>
#include <thread>

// ================= 0. (Optional) For convenience include fsm.hpp and do a 'using' on namespace ====================

// 0.1
// fsm.hpp includes all other headers mentioned below
#include <chestnut/fsm/fsm.hpp>
// 0.2
// Writing this long two-part namespace every time can be annoying :) 
using namespace chestnut::fsm; 





// ====================================== 1. Define your statemachine type ============================================

// 1.1. (Optional)
// Create a state extension class.
// By default a statemachine class has access only to the very basic state methods like canEnterState.
// States will still be able to transition between each other, the statemachine class will simply not be able to interact with them in any more custom manner.
// An extension class will allow you to use more than these basic methods and set up a way to communicate with states.
// Extension can have pure virtual methods for example so that states will actually be forced to implement them.
// By creating a statemachine with state extension you will be able to call public methods of that extension on states
class DoorStateExtension
{
public:
    virtual bool tryOpen() = 0;
    virtual bool tryClose() = 0;
};

// 1.2. 
// To create a custom statemachine class include statemachine.hpp header

#include <chestnut/fsm/statemachine.hpp>

// ...If you decided to use a state extension class, in the template parameter of chestnut::fsm::Statemachine
// input this type. Otherwise you can just write chestnut::fsm::Statemachine<> for a basic setup
class CDoorStatemachine : public Statemachine<DoorStateExtension>
{
public:
    // 1.3. (Optional) 
    // You can make your statemachine able to be used across threads in an async manner
    // The base Statemachine type does not support multithreading, so you'll have to set up the necessary precautions yourself
    mutable std::mutex doorMutex;


    CDoorStatemachine();


    // 1.4. (Optional) If you want you can create methods in the statemachine with the same signatures that you gave in point 1.1
    // This will make them look as a sort of "state-virtual" methods, 
    // i.e. methods which behaviour depends almost solely on the current state of the statemachine

    bool tryOpen()
    {
        std::lock_guard<std::mutex> lock( doorMutex );
        return getCurrentState()->tryOpen();
    }

    bool tryClose()
    {
        std::lock_guard<std::mutex> lock( doorMutex );
        return getCurrentState()->tryClose();
    }
};




// ====================================== 2. Define your states ======================================

// 2.1. Be aware of the order of your includes!
//
// Calls to initState(), gotoState() and pushState() require that the state type given to them in the template parameter is a *complete* type.
// If you need a mutual state dependency (e.g. state A goes to state B and vice versa) it's best to properly divide
// state class body and its methods' definitions.
// You can't make state transition to a forward declared state!
// If you decide to write a statemachine it'd be a lot better to keep each state (and the statemachine itself) in seperate .h and .cpp files 
//
// More on incomplete types: https://en.cppreference.com/w/cpp/language/type#Incomplete_type

// 2.2
// To create a state class first include state.hpp header

#include <chestnut/fsm/state.hpp>

// ...then inherit from chestnut::fsm::State 
// for the template parameter give it the type of the statemachine you created
// The base state type will be deduced from a typedef inside the statemachine class
class CDoorStateClosed : public State<CDoorStatemachine>
{
public:
    // 2.3. (Optional) 
    // States can have a custom constructor or just a default one
    // You pass these custom parameters when you call initState(), gotoState() or pushState()
    CDoorStateClosed( bool printOnNullState = false );

    // 2.4. (Optional) 
    // Override methods that will be called when a statemachine enter or leaves the state
    // The data about the transition (whether it was done with popState() method for example) is in StateTransition struct in the parameter
    // You are not obligated to override these methods however
    void onEnterState( StateTransition transition ) override;
    void onLeaveState( StateTransition transition ) override;
    
    // 2.5. (Optional) 
    // Override the virtual methods you specified in point 1.1
    bool tryOpen() override;
    bool tryClose() override;

private:
    bool printOnNullState;
};

class CDoorStateOpening : public State<CDoorStatemachine>
{
public:
    // 2.6. 
    // You don't even need to define a constructor at all if you don't need to!

    void onEnterState( StateTransition transition ) override;
    void onLeaveState( StateTransition transition ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
};

class CDoorStateOpen : public State<CDoorStatemachine>
{
public:
    void onEnterState( StateTransition transition ) override;
    void onLeaveState( StateTransition transition ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
};

class CDoorStateClosing : public State<CDoorStatemachine>
{
public:
    void onEnterState( StateTransition transition ) override;
    void onLeaveState( StateTransition transition ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
};





CDoorStatemachine::CDoorStatemachine() 
{
    // 2.7. 
    // A statemachine has to be initialized with some entry state
    // This state will stay on the state stack throughout the lifetime of the statemachine and won't be possible to get poppped
    // If you don't have a state that could be good candidate for a state that the machine should always eventually transition back to
    // you can simply create an empty state! Just inherit from the base state type and that's it. 
    // State does not force you to override any pure virtual methods or create any special constructors.
    //
    // Based on point 3.1 if we wanted to use a state transition method on CDoorStateClosed we have to use it AFTER its complete type body.
    // Hence why CDoorStatemachine() constructor definition is all the way here, but it's only because we're working in a single .cpp file.

    // 2.8. (Optional) 
    // We can call state transition method with custom parameters. These parameters will be forwarded to state's constructor. See point 3.3.
    // Calling initState() inside a constructor ensures it will always be initialised with this state
    initState<CDoorStateClosed>( true );
}





CDoorStateClosed::CDoorStateClosed( bool printOnNullState )
{
    // 2.7. 
    // Inside a state constructor you shouldn't use the getParent() method, because the pointer to the parent statemachine has not been set up by this point
    // Calling it here will throw BadParentAccessException
    // If you want to use the parent statemchine do it in onEnterState and onLeaveState methods instead
    this->printOnNullState = printOnNullState;
}

void CDoorStateClosed::onEnterState( StateTransition transition )
{
    // message won't show if this is the init state
    if( transition.type != STATE_TRANSITION_INIT )
    {
        std::cout << "The door is now closed!\n";
    }
    else if( printOnNullState )
    {
        std::cout << "CDoorStateClosed is the init state!\n";
    }
}

void CDoorStateClosed::onLeaveState( StateTransition transition )
{
    // message won't show if statemachine is being deleted
    if( transition.type != STATE_TRANSITION_DESTROY )
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
    getParent().pushState<CDoorStateOpening>();
    return true;  
}

bool CDoorStateClosed::tryClose()
{
    std::cout << "The door is already closed!\n";
    return false;
}



void CDoorStateOpening::onEnterState( StateTransition transition ) 
{
    std::cout << "The door is openning...\n";

    // spawn a thread that'll wait 2 seconds and then transition to next state
    std::thread( [this] {
        std::this_thread::sleep_for( std::chrono::seconds(2) );
        // we're in a different thread, we have to lock the parent
        std::lock_guard<std::mutex> lock( getParent().doorMutex );
        getParent().gotoState<CDoorStateOpen>();
    }).detach();
}

void CDoorStateOpening::onLeaveState( StateTransition transition ) 
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



void CDoorStateOpen::onEnterState( StateTransition transition ) 
{
    // message won't show if this is the init state
    if( transition.type != STATE_TRANSITION_INIT )
    {
        std::cout << "The door is now open!\n";
    }
}

void CDoorStateOpen::onLeaveState( StateTransition transition ) 
{
    // message won't show if statemachine is being deleted
    if( transition.type != STATE_TRANSITION_DESTROY )
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
    getParent().gotoState<CDoorStateClosing>();
    return true;
}



void CDoorStateClosing::onEnterState( StateTransition transition ) 
{
    std::cout << "The door is closing...\n";

    std::thread( [this] {
        std::this_thread::sleep_for( std::chrono::seconds(2) );
        // we're in a different thread, we have to lock the parent
        std::lock_guard<std::mutex> lock( getParent().doorMutex );
        getParent().popState();
    }).detach();
}

void CDoorStateClosing::onLeaveState( StateTransition transition )
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

// ===================== 3. Use your statemachine object =====================

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
        }

        printDoorState();

        if( door.tryClose() )
        {
            printDoorState();

            while( door.isCurrentlyInState<CDoorStateClosing>() )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds(100) );
            }

            printDoorState();
        }
    }

    return 0;
}

/* CONSOLE OUTPUT
CDoorStateClosed is the init state!
Door state: Closed; state stack size: 1
The door is already closed!
The door is no longer closed!
The door is openning...
Door state: Opening; state stack size: 2
The door has finished openning!
The door is now open!
Door state: Open; state stack size: 2
The door is no longer open!
The door is closing...
Door state: Closing; state stack size: 2
The door has finished closing!
The door is now closed!
Door state: Closed; state stack size: 1
Statemachine is being destroyed!
*/
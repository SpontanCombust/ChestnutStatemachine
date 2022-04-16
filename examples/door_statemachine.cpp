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


// 1.2. (IMPORTANT!)
// A thing or two about cyclic dependency:
// All state change methods like gotoState() or initState() take state type name as the template parameter. This and also the fact that 
// underneath a instance of this class is created means that the type you give to these methods has to be a *complete type*. 
// What this in turn means is that there has to be a declaration of said type before its usage and somewhere in an entire translation unit where it happens
// there has to be a body of this state type. To avoid the issue of cyclic header inclusion you can use forward class declaration as shown above. 
// This is C++ and this is a thing we all have to remember and deal with unfortunatelly. 
// More on incomplete types: https://en.cppreference.com/w/cpp/language/type#Incomplete_type
//
// For this reason, because we will want to use CDoorStateClosed below here to initialize the statemachine we will forward declare its type.
// You will see more examples of such action further below.
class CDoorStateClosed; // forward declaration because of usage in CDoorStatemachine::CDoorStatemachine()


// 1.3. 
// To create a custom statemachine class include statemachine.hpp header

#include <chestnut/fsm/statemachine.hpp>

// ...If you decided to use a state extension class, in the template parameter of chestnut::fsm::Statemachine
// input this type. Otherwise you can just write chestnut::fsm::Statemachine<> for a basic setup
class CDoorStatemachine : public Statemachine<DoorStateExtension>
{
public:
    // 1.4. (Optional) 
    // You can make your statemachine able to be used across threads in an async manner
    // The base Statemachine type does not support multithreading, so you'll have to set up the necessary precautions yourself
    mutable std::mutex doorMutex;


    CDoorStatemachine()
    {
        // 1.5. 
		// A statemachine has to be initialized with some entry state
		// This state will stay on the state stack throughout the lifetime of the statemachine and won't be possible to get poppped
		// If you don't have a state that could be good candidate for a state that the machine should always eventually transition back to
		// you can simply create an empty state! Just inherit from the base state type and that's it. 
		// State does not force you to override any pure virtual methods or create any special constructors.
		//
		// 1.6. (Optional) 
		// We can call state transition method with custom parameters. These parameters will be forwarded to state's constructor. See point 3.3.
		// Calling initState() inside a constructor ensures it will always be initialised with this state
		initState<CDoorStateClosed>( true );
    }


    // 1.7. (Optional) If you want you can create methods in the statemachine with the same signatures that you gave in point 1.1
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

// 2.1
// To create a state class first include state.hpp header

#include <chestnut/fsm/state.hpp>


class CDoorStateOpening; // forward declaration because of usage in CDoorStateClosed::tryOpen()

// ...then inherit from chestnut::fsm::State 
// for the template parameter give it the type of the statemachine you created
// The base state type will be deduced from a typedef inside the statemachine class
class CDoorStateClosed : public State<CDoorStatemachine>
{
public:
    // 2.2. (Optional) 
    // States can have a custom constructor or just a default one
    // You pass these custom parameters when you call initState(), gotoState() or pushState()
    CDoorStateClosed( bool printOnNullState = false )
    {
        // 2.3. 
        // Inside a state constructor you shouldn't use the getParent() method, because the pointer to the parent statemachine has not been set up by this point
        // Calling it here will throw BadParentAccessException
        // If you want to use the parent statemchine do it in onEnterState and onLeaveState methods instead
        this->printOnNullState = printOnNullState;
    }

    // 2.4. (Optional) 
    // Override methods that will be called when a statemachine enter or leaves the state
    // The data about the transition (whether it was done with popState() method for example) is in StateTransition struct in the parameter
    // You are not obligated to override these methods however
    void onEnterState( StateTransition transition ) override
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

    void onLeaveState( StateTransition transition ) override
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
    
    // 2.5. (Optional) 
    // Override the virtual methods you specified in point 1.1
    bool tryOpen() override
	{
		// State change methods work based on state stack
		// initState initializes the state stack
		// pushState pushes a new state onto a stack and transitions into it
		// popState pops a state from the top of a stack and transitions to the one below
		// gotoState replaces the state on top of the stack by transitioning to the new state directly
		//
		// There is an exception to these rules above - that is the initial state.
		// Init state cannot be popped and if you call a gotoState on a statemachine with only the init state it will behave like pushState
		getParent().pushState<CDoorStateOpening>();
    	return true;
	}

    bool tryClose() override
	{
		std::cout << "The door is already closed!\n";
    	return false;
	}

private:
    bool printOnNullState;
};






class CDoorStateOpen; // forward declaration because of usage in CDoorStateOpening::onEnterState()

class CDoorStateOpening : public State<CDoorStatemachine>
{
public:
    // 2.6. 
    // You don't even need to define a constructor at all if you don't need to!


    void onEnterState( StateTransition transition ) override
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

    void onLeaveState( StateTransition transition ) override
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






class CDoorStateClosing; // forward declaration because of usage in CDoorStateOpen::tryClose()

class CDoorStateOpen : public State<CDoorStatemachine>
{
public:
    void onEnterState( StateTransition transition ) override
	{
		// message won't show if this is the init state
		if( transition.type != STATE_TRANSITION_INIT )
		{
			std::cout << "The door is now open!\n";
		}
	}

    void onLeaveState( StateTransition transition ) override
	{
		// message won't show if statemachine is being deleted
		if( transition.type != STATE_TRANSITION_DESTROY )
		{
			std::cout << "The door is no longer open!\n";
		}
	}
    
    bool tryOpen() override
	{
		std::cout << "The door is already open!\n";     
    	return false;
	}

    bool tryClose() override
	{
		getParent().gotoState<CDoorStateClosing>();
    	return true;
	}
};







class CDoorStateClosing : public State<CDoorStatemachine>
{
public:
    void onEnterState( StateTransition transition ) override
	{
		std::cout << "The door is closing...\n";

		std::thread( [this] {
			std::this_thread::sleep_for( std::chrono::seconds(2) );
			// we're in a different thread, we have to lock the parent
			std::lock_guard<std::mutex> lock( getParent().doorMutex );
			getParent().popState();
		}).detach();
	}

    void onLeaveState( StateTransition transition ) override
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








// ===================== 3. Use your statemachine object =====================

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
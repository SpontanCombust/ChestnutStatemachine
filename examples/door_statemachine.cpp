#include "../include/chestnut/statemachine/state.hpp"
#include "../include/chestnut/statemachine/statemachine.hpp"

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>




// ===================== 1. Forward declare statemachine type and define the state interface =====================

class CDoorStatemachine;

class IDoorState : public chestnut::fsm::IState<CDoorStatemachine>
{
public:
    typedef chestnut::fsm::IState<CDoorStatemachine> super;

    // 1.1 You have to define a constructor that at least takes in parent statemachine pointer (this pointer has to always be first constructor argument)
    IDoorState( CDoorStatemachine *sm ) : super( sm ) {}

    // Declare any number of virtual methods that your state classes should implement
    virtual bool tryOpen() = 0;
    virtual bool tryClose() = 0;
};




// ====================================== 2. Define your statemachine type ======================================
class CDoorStatemachine : public chestnut::fsm::IStatemachine<IDoorState>
{
public:
    // utility parent class typedef
    typedef chestnut::fsm::IStatemachine<IDoorState> super;

    // You can make your statemachine able to be used across threads in an async manner
    // You have to be very careful with this however, like with any other scenario that involves race conditions
    // The base IStatemachine type does not automatically support multithreading
    mutable std::recursive_mutex doorMutex;


    // Define methods that will call appropriate method in the current state

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




// ====================================== 3. Define your states ======================================

// If you put state interface and statemachine definition in seperate headers 
// you'll want to include them both in states' headers if you want to use the parent pointer
//
// Be aware of the include order - if you need a mutual state dependency (e.g. state A goes to state B and vice versa)
// it's best to properly devide state class body and its methods' definitions
// You can't make state transition to forward declared, uncomplete classes
class CDoorStateClosed : public IDoorState
{
public:
    // Again we have to define a special constructor
    // Here we're also adding an additional custom parameter
    CDoorStateClosed( CDoorStatemachine *sm, bool printOnNullState = false );

    void onEnter( std::type_index prevState ) override;
    void onExit( std::type_index nextState ) override;
    
    bool tryOpen() override;
    bool tryClose() override;

private:
    bool printOnNullState;
};

class CDoorStateOpening : public IDoorState
{
public:
    // Again we have to define a special constructor
    CDoorStateOpening( CDoorStatemachine *sm );

    void onEnter( std::type_index prevState ) override;
    void onExit( std::type_index nextState ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
};

class CDoorStateOpen : public IDoorState
{
public:
    // Again we have to define a special constructor
    CDoorStateOpen( CDoorStatemachine *sm );

    void onEnter( std::type_index prevState ) override;
    void onExit( std::type_index nextState ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
};

class CDoorStateClosing : public IDoorState
{
public:
    // Again we have to define a special constructor
    CDoorStateClosing( CDoorStatemachine *sm );

    void onEnter( std::type_index prevState ) override;
    void onExit( std::type_index nextState ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
};



using chestnut::fsm::NULL_STATE;

CDoorStateClosed::CDoorStateClosed( CDoorStatemachine *sm, bool printOnNullState ) : IDoorState( sm ) 
{
    this->printOnNullState = printOnNullState;
}

void CDoorStateClosed::onEnter( std::type_index prevState )
{
    // message won't show if this is the init state
    if( prevState != NULL_STATE )
    {
        std::cout << "The door is now closed!\n";
    }
    else if( printOnNullState )
    {
        std::cout << "CDoorStateClosed is the init state!\n";
    }
}

void CDoorStateClosed::onExit( std::type_index nextState )
{
    // message won't show if statemachine is being deleted
    if( nextState != NULL_STATE )
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



CDoorStateOpening::CDoorStateOpening( CDoorStatemachine *sm ) : IDoorState( sm ) 
{

}

void CDoorStateOpening::onEnter( std::type_index prevState ) 
{
    std::cout << "The door is openning...\n";

    // spawn a thread that'll wait 2 seconds and then transition to next state
    std::thread( [this] {
        std::this_thread::sleep_for( std::chrono::seconds(2) );
        std::lock_guard< std::recursive_mutex > lock( parent->doorMutex );
        parent->gotoState<CDoorStateOpen>();
    }).detach();
}

void CDoorStateOpening::onExit( std::type_index nextState ) 
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



CDoorStateOpen::CDoorStateOpen( CDoorStatemachine *sm ) : IDoorState( sm ) 
{

}

void CDoorStateOpen::onEnter( std::type_index prevState ) 
{
    // message won't show if this is the init state
    if( prevState != NULL_STATE )
    {
        std::cout << "The door is now open!\n";
    }
}

void CDoorStateOpen::onExit( std::type_index nextState ) 
{
    // message won't show if statemachine is being deleted
    if( nextState != NULL_STATE )
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



CDoorStateClosing::CDoorStateClosing( CDoorStatemachine *sm ) : IDoorState( sm ) 
{

}

void CDoorStateClosing::onEnter( std::type_index prevState ) 
{
    std::cout << "The door is closing...\n";

    std::thread( [this] {
        std::this_thread::sleep_for( std::chrono::seconds(2) );
        std::lock_guard< std::recursive_mutex > lock( parent->doorMutex );
        parent->popState();
    }).detach();
}

void CDoorStateClosing::onExit( std::type_index nextState )
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

    door.init<CDoorStateClosed>( true ); // using custom state constructor parameter

    printDoorState();
    if( door.tryClose() )
    {
        printDoorState();
    }
    if( door.tryOpen() )
    {
        printDoorState();

        std::type_index state = door.getCurrentStateType();
        while( state == typeid(CDoorStateOpening) )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds(100) ); // we'll keep waiting small intervals until the door fully opens

            std::lock_guard< std::recursive_mutex > lock( door.doorMutex );
            state = door.getCurrentStateType();
        }

        printDoorState();

        if( door.tryClose() )
        {
            printDoorState();

            state = door.getCurrentStateType();
            while( state == typeid(CDoorStateClosing) )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds(100) );

                std::lock_guard< std::recursive_mutex > lock( door.doorMutex );
                state = door.getCurrentStateType();
            }

            printDoorState();
        }
    }

    return 0;
}

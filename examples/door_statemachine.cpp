#include "../include/chestnut/statemachine/state.hpp"
#include "../include/chestnut/statemachine/statemachine.hpp"

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

using namespace chestnut::statemachine;





// 1. Forward declare statemachine type and define the state interface
class CDoorStatemachine;

class IDoorState : public IState<CDoorStatemachine>
{
public:
    typedef IState<CDoorStatemachine> super;

    // 2.1 You have to define the special constructor that takes in statemachine pointer, you can pass the pointer to parent class constructor
    // At the moment this library doesn't support custom state constructors
    IDoorState( CDoorStatemachine *sm ) : super( sm ) {}

    virtual bool tryOpen() = 0;
    virtual bool tryClose() = 0;
};




// 2. Define your statemachine type
class CDoorStatemachine : public IStatemachine<IDoorState>
{
public:
    // utility parent class typedef
    typedef IStatemachine<IDoorState> super;

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

    void gotoState( std::type_index nextStateType ) override
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        super::gotoState( nextStateType );
    }

    void pushState( std::type_index nextStateType ) override
    {
        std::lock_guard< std::recursive_mutex > lock( doorMutex );
        super::pushState( nextStateType );
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




// 3. Define your states
//
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
    CDoorStateClosed( CDoorStatemachine *sm );

    void onEnter( std::type_index prevState ) override;
    void onExit( std::type_index nextState ) override;
    
    bool tryOpen() override;
    bool tryClose() override;
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



CDoorStateClosed::CDoorStateClosed( CDoorStatemachine *sm ) : IDoorState( sm ) 
{

}

void CDoorStateClosed::onEnter( std::type_index prevState )
{
    // guard in case this is the default state
    if( prevState != typeid(CDoorStateClosed) )
    {
        std::cout << "The door is now closed!\n";
    }
}

void CDoorStateClosed::onExit( std::type_index nextState )
{
    std::cout << "The door is no longer closed!\n";
}

bool CDoorStateClosed::tryOpen()
{
    parent->gotoState( typeid(CDoorStateOpening) );   
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

    std::thread( [this] {
        std::this_thread::sleep_for( std::chrono::seconds(2) );
        parent->gotoState( typeid(CDoorStateOpen) );
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
    // guard in case this is the default state
    if( prevState != typeid(CDoorStateOpen) )
    {
        std::cout << "The door is now open!\n";
    }
}

void CDoorStateOpen::onExit( std::type_index nextState ) 
{
    std::cout << "The door is no longer open!\n";
}

bool CDoorStateOpen::tryOpen() 
{
    std::cout << "The door is already open!\n";     
    return false;
}

bool CDoorStateOpen::tryClose()
{
    parent->gotoState( typeid(CDoorStateClosing) );
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
        parent->gotoState( typeid(CDoorStateClosed) );
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

// 5. Create a statemachine object
int main(int argc, char const *argv[])
{
    CDoorStatemachine door;
    door.setupStates< CDoorStateClosed, CDoorStateOpen, CDoorStateClosing, CDoorStateOpening >();

    auto printDoorState = [&door] {
        std::cout << "Door state: " << doorStateTypeToString( door.getCurrentStateType() ) << "\n";
    };


    printDoorState();
    if( door.tryClose() )
    {
        printDoorState();
    }
    if( door.tryOpen() )
    {
        printDoorState();
        while( door.getCurrentStateType() == typeid(CDoorStateOpening) )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds(100) );
        }
        printDoorState();

        if( door.tryClose() )
        {
            printDoorState();
            while( door.getCurrentStateType() == typeid(CDoorStateClosing) )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds(100) );
            }
            printDoorState();
        }
    }

    return 0;
}

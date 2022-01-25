#include "exceptions.hpp"

namespace chestnut::fsm
{

StateBase::StateBase() 
{
    // so that use of this pointer in a constructor can be detected reliably
    this->parent = nullptr;
}

bool StateBase::setParent( StatemachineBase *parent_ ) noexcept
{
    // children classes will simply do dynamic_cast on their desired statemachine type
    this->parent = parent;
    return true;
}

inline StatemachineBase& StateBase::getParent()
{
    if( parent )
    {
        return *parent;
    }
    else
    {
        throw BadParentAccessException( "State parent access violation!");
    }
}

inline const StatemachineBase& StateBase::getParent() const
{
    if( parent )
    {
        return *parent;
    }
    else
    {
        throw BadParentAccessException( "State parent access violation!");
    }
}

inline bool StateBase::canEnterState( StateTransition transition ) const noexcept
{
    return true;
}

inline bool StateBase::canLeaveState( StateTransition transition ) const noexcept
{
    return true;
}

inline void StateBase::onEnterState( StateTransition transition ) 
{
    /*NOP*/
}

inline void StateBase::onLeaveState( StateTransition transition ) 
{
    /*NOP*/
}

} // namespace chestnut::fsm

#include "exceptions.hpp"

namespace chestnut::fsm
{

template<class ParentStatemachineClass, class BaseStateClass>
typename State<ParentStatemachineClass, BaseStateClass>::StatemachineType& State<ParentStatemachineClass, BaseStateClass>::getParent() 
{
    // setParent ensures to the statemachine the parent pointer is of valid type 
    // so this will either be null or valid StatemachineType
    if( this->parent )
    {
        return *static_cast<StatemachineType*>( this->parent );
    }
    else
    {
        throw BadParentAccessException( "State parent access violation!");
    }
}

template<class ParentStatemachineClass, class BaseStateClass>
const typename State<ParentStatemachineClass, BaseStateClass>::StatemachineType& State<ParentStatemachineClass, BaseStateClass>::getParent() const 
{
    if( this->parent )
    {
        return *static_cast<StatemachineType*>( this->parent );
    }
    else
    {
        throw BadParentAccessException( "State parent access violation!");
    }
}

template<class ParentStatemachineClass, class BaseStateClass>
bool State<ParentStatemachineClass, BaseStateClass>::setParent( StatemachineBase *parent_ ) noexcept
{
    if( dynamic_cast<StatemachineType*>( parent_ ) )
    {
        this->parent = parent_;
        return true;
    }

    return false;
}


template<class ParentStatemachineClass>
typename State<ParentStatemachineClass, void>::StatemachineType& State<ParentStatemachineClass, void>::getParent() 
{
    if( this->parent )
    {
        return *static_cast<StatemachineType*>( this->parent );
    }
    else
    {
        throw BadParentAccessException( "State parent access violation!");
    }
}

template<class ParentStatemachineClass>
const typename State<ParentStatemachineClass, void>::StatemachineType& State<ParentStatemachineClass, void>::getParent() const 
{
    if( this->parent )
    {
        return *static_cast<StatemachineType*>( this->parent );
    }
    else
    {
        throw BadParentAccessException( "State parent access violation!");
    }
}

template<class ParentStatemachineClass>
bool State<ParentStatemachineClass, void>::setParent( StatemachineBase *parent_ ) noexcept
{
    if( dynamic_cast<StatemachineType*>( parent_ ) )
    {
        this->parent = parent_;
        return true;
    }

    return false;
}

} // namespace chestnut::fsm

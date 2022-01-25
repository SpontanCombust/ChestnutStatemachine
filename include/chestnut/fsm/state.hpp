#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

#include "state_base.hpp"
#include "statemachine.hpp"

namespace chestnut::fsm
{

// When you want to extend a state of a base statemachine class
template< class ParentStatemachineClass, class BaseStateClass = void >
class State : virtual public ParentStatemachineClass::BaseStateType, virtual public BaseStateClass
{
public:
    typedef ParentStatemachineClass StatemachineType;
    typedef class ParentStatemachineClass::BaseStateType BaseStateType;

public:
    // Don't call in constructor! BadParentAccessException
    virtual StatemachineType& getParent() override;
    // Don't call in constructor! BadParentAccessException
    virtual const StatemachineType& getParent() const override;

private:
    // Returns whether this state type can be bound to a given statemachine type
    virtual bool setParent( StatemachineBase *parent_ ) noexcept override;
};

// When you just want a state for a statemachine
template< class ParentStatemachineClass >
class State<ParentStatemachineClass, void> : virtual public ParentStatemachineClass::BaseStateType
{
public:
    typedef ParentStatemachineClass StatemachineType;
    typedef class ParentStatemachineClass::BaseStateType BaseStateType;

public:
    // Don't call in constructor! BadParentAccessException
    virtual StatemachineType& getParent() override;
    // Don't call in constructor! BadParentAccessException
    virtual const StatemachineType& getParent() const override;

private:
    // Returns whether this state type can be bound to a given statemachine type
    virtual bool setParent( StatemachineBase *parent_ ) noexcept override;
};

} // namespace chestnut::fsm


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
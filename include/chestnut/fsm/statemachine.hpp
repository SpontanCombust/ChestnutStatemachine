#ifndef __CHESTNUT_STATEMACHINE_STATEMACHINE_H__
#define __CHESTNUT_STATEMACHINE_STATEMACHINE_H__

#include "statemachine_base.hpp"

namespace chestnut::fsm
{

template< class StateExtension = void, class BaseStatemachineClass = chestnut::fsm::StatemachineBase >
class Statemachine : public BaseStatemachineClass
{
public:
    class BaseStateClass :  virtual public chestnut::fsm::StateBase,
                            virtual public StateExtension
    { 
        
    };
    
    typedef BaseStatemachineClass BaseStatemachineType;

    typedef BaseStateClass BaseStateType;

    typedef StateExtension StateExtensionType;


public:
    Statemachine();

    BaseStateType *getCurrentState() const noexcept;
};

// This can be achieved by regular inheritance of the custom base statemachine class
// This template specialisation is still set up here so that new BaseStateClass is not created and it doesn't try to inherit from void
// BaseStateType & StateExtensionType are set using the typedef from parent class, that is BaseStatemachineClass
template< class BaseStatemachineClass >
class Statemachine<void,BaseStatemachineClass> : public BaseStatemachineClass
{
public:
    typedef BaseStatemachineClass BaseStatemachineType;

    typedef class BaseStatemachineClass::BaseStateType BaseStateType;

    typedef class BaseStatemachineClass::StateExtensionType StateExtensionType;


public:
    Statemachine();
};

// This can be achieved by regular inheritance of chestnut::fsm::StatemachineBase
// This template specialisation is still set up here so that new BaseStateClass is not created and it doesn't try to inherit from void
// BaseStateType & StateExtensionType are set using the typedef from chestnut::fsm::StatemachineBase
template<>
class Statemachine<void,chestnut::fsm::StatemachineBase> : public chestnut::fsm::StatemachineBase
{
public:
    typedef chestnut::fsm::StatemachineBase BaseStatemachineType;

    typedef chestnut::fsm::StatemachineBase::BaseStateType BaseStateType;

    typedef chestnut::fsm::StatemachineBase::StateExtensionType StateExtensionType;
};

} // namespace chestnut::fsm


#include "statemachine.inl"


#endif // __CHESTNUT_STATEMACHINE_STATEMACHINE_H__
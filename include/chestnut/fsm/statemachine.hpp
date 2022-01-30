/**
 * @file statemachine.hpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Header file with the template Statemachine class
 * @version 3.0.0
 * @date 2022-01-30
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */


#ifndef __CHESTNUT_STATEMACHINE_STATEMACHINE_H__
#define __CHESTNUT_STATEMACHINE_STATEMACHINE_H__

#include "statemachine_base.hpp"

namespace chestnut::fsm
{

/**
 * @brief Template statemachine class. Inherit from this type to create a proper statemachine class
 * 
 * @details
 * When this template type is created it automatically inherits from provided BaseStatemachineClass.
 * It also creates a nested base state from which all states that want to belong to this statemachine will derive from.
 * This base state class is created via inheriting from both chestnut::fsm::StateBase for the basic state methods
 * and from StateExtension to get all the functionality that user wants.
 * If you provide the extension type AND the BaseStatemachineClass as not chestnut::fsm::StatemachineBase,
 * then StateExtension should be a subclass of the extension type from BaseStatemachineClass.
 * The special case here is when doesn't want any new state extension. For this template specializations were created.
 * 
 * @tparam StateExtension 
 * @tparam BaseStatemachineClass 
 */
template< class StateExtension = void, class BaseStatemachineClass = chestnut::fsm::StatemachineBase >
class Statemachine : public BaseStatemachineClass
{
public:
    /**
     * @brief Base state class that all state classes in this statemachine will derive from
     */
    class BaseStateClass :  virtual public chestnut::fsm::StateBase,
                            virtual public StateExtension
    { 
        
    };

    /**
     * @brief Typedef of the parent class of this class; overrides the typedef from the parent class
     */
    typedef BaseStatemachineClass BaseStatemachineType;
    /**
     * @brief Typedef of the BaseStateClass; overrides the typedef from the parent class
     */
    typedef BaseStateClass BaseStateType;
    /**
     * @brief Typedef of the state extension type; overrides the typedef from the parent class
     */
    typedef StateExtension StateExtensionType;


public:
    /**
     * @brief Constructor
     * 
     */
    Statemachine();

    /**
     * @brief Get the pointer to a state object on top of the state stack or nullptr if statemachine was not initialized
     * 
     * @return pointer to current state, upcasted to base statemachine state type
     * 
     * 
     * @see initState()
     */
    BaseStateType *getCurrentState() const noexcept;
};


// This template specialisation is still set up here so that new BaseStateClass is not created and it doesn't try to inherit from void
// BaseStateType & StateExtensionType are set using the typedef from parent class, that is BaseStatemachineClass
// This here can be achieved by regular inheritance of the custom base statemachine class
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


// This template specialisation is still set up here so that new BaseStateClass is not created and it doesn't try to inherit from void
// BaseStateType & StateExtensionType are set using the typedef from chestnut::fsm::StatemachineBase
// This here can be achieved by regular inheritance of chestnut::fsm::StatemachineBase
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
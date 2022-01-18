/**
 * @file state.hpp
 * @author SpontanCombust
 * @brief Header file with base template state class and NULL_STATE constant
 * @version 2.3.0
 * @date 2022-01-10
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */


#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

#include <typeindex>

namespace chestnut::fsm
{

/**
 * @brief Base generic class used for creating state interface
 * 
 * @tparam ParentStatemachine type of the statemachine that will house the state
 * 
 * @details
 * ParentStatemachine type should be forward declared before the state interface.
 */
template< class ParentStatemachine >
class IState
{
protected:
    /**
     * @brief Pointer to the statemachine that houses the state
     */
    ParentStatemachine *parent;

public:
    typedef ParentStatemachine* ParentStatemachinePtrType;

public:
    IState( ParentStatemachine *parent_ );
    virtual ~IState() = default;
    
    /**
     * @brief A method called whenever statemachine enters this state
     * 
     * @param prevState type of the state statemachine swithed from
     */
    virtual void onEnterState( std::type_index prevState ) {}

    /**
     * @brief A method called whenever statemachine leaves this state
     * 
     * @details
     * gotoState, pushState and popState calls coming from this methods are ignored to prevent undefined behaviour. 
     * 
     * @param nextState type of the state statemachine is swithing to
     */
    virtual void onLeaveState( std::type_index nextState ) {}
};


/**
 * @brief A constant type index meant to represent a lack of state
 * 
 * @details
 * There are currently 2 situations when NULL_STATE is used: \n 
 * 1. When statemachine is being initialized with its init state, then NULL_STATE is passed to onEnterState of that state \n
 * 2. When statemachine is being deleted, then NULL_STATE is passed to onLeaveState of every state on the state stack before deleting them
 */
const std::type_index NULL_STATE = std::type_index( typeid(nullptr) );


} // namespace chestnut::fsm


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
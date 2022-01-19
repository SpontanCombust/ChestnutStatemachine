/**
 * @file state.hpp
 * @author SpontanCombust
 * @brief Header file with base template state class
 * @version 2.3.0
 * @date 2022-01-10
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */

#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

#include "state_base.hpp"

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
class IState : public IStateBase
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
};

} // namespace chestnut::fsm


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
/**
 * @file state.hpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Header file with the state interface template used to create a base state class
 * @version 3.0.0
 * @date 2022-01-20
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
 * @brief State interface template used for creating a base state class
 * 
 * @tparam ParentStatemachineClass type of the statemachine that will house the state
 * 
 * @details
 * ParentStatemachineClass should be forward declared before the state interface.
 */
template< class ParentStatemachineClass >
class IState : public IState_Base
{
protected:
    /**
     * @brief Pointer to the statemachine object that houses the state
     */
    ParentStatemachineClass *parent;


public:
    /**
     * @brief Typedef of parent statemachine class to be used as class member type
     */
    typedef ParentStatemachineClass StatemachineType;
    /**
     * @brief Typedef of parent statemachine class pointer be used as class member type
     */
    typedef ParentStatemachineClass* StatemachinePtrType;


public:
    /**
     * @brief IState constructor
     */
    IState() noexcept;

    // @brief Sets the pointer to the parent statemachine object.
    //
    // THIS METHOD IS USED INTERNALLY AND SHOULD NOT BE CALLED BY THE USER!
    //
    // @param parent pointer to parent statemachine
    void setParent( StatemachinePtrType parent ) noexcept;
};

} // namespace chestnut::fsm


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
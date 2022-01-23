/**
 * @file state.hpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Header file with the template state type used to create a state class with an access to the parent statemachine
 * @version 3.0.0
 * @date 2022-01-20
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */

#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

#include "state_base.hpp"
#include "statemachine.hpp"

namespace chestnut::fsm
{

/**
 * @brief Template state type used to create a state class with an access to the parent statemachine; it derives from fsm::StateBase
 * 
 * @tparam ParentStatemachineClass type of the statemachine that will be this state's parent
 * 
 */
template< class ParentStatemachineClass >
class State : public ParentStatemachineClass::BaseStateType
{
public:
    /**
     * @brief Typedef of parent statemachine class to be used as class member type
     */
    typedef ParentStatemachineClass StatemachineType;
    /**
     * @brief Typedef of parent statemachine class pointer be used as class member type
     */
    typedef ParentStatemachineClass* StatemachinePtrType;

    /**
     * @brief Make the template Statemachine a friend of this template class
     */
    friend Statemachine<typename ParentStatemachineClass::BaseStateType>;


protected:
    /**
     * @brief Pointer to the statemachine object that houses the state
     */
    ParentStatemachineClass *parent;


public:
    /**
     * @brief State constructor
     */
    State() noexcept;


private:
    /**
     * @brief Sets the pointer to the parent statemachine object.
     * Thanks to the friendship with Statemachine template it can be called from that and only that type.
     * 
     * @param parent pointer to parent statemachine 
     */
    void setParent( StatemachinePtrType parent ) noexcept;
};

} // namespace chestnut::fsm


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
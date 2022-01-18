/**
 * @file statemachine.hpp
 * @author SpontanCombust
 * @brief Header file with base template statemachine type
 * @version 2.2.0
 * @date 2021-11-28
 * 
 * @copyright MIT License (c) 2021
 * 
 */


#ifndef __CHESTNUT_STATEMACHINE_STATEMACHINE_H__
#define __CHESTNUT_STATEMACHINE_STATEMACHINE_H__

#include "state.hpp"
#include "exceptions.hpp"

#include <stack>
#include <typeindex>

namespace chestnut::fsm
{
    
/**
 * @brief Base generic class for creating a statemachine
 * 
 * @tparam StateInterface a base (interface) class from which all states inherit
 * 
 * 
 * @details
 * StateInterface type must inherit from IState to work properly.
 * It can have any number of virtual methods that proper states should implement in some form.
 * 
 * @see IState
 */
template< typename StateInterface >
class IStatemachine
{
private:
    /**
     * @brief A stack of state pointers
     */
    std::stack< StateInterface* > m_stackStates;
    /**
     * @brief A flag set to prevent onLeaveState from calling state change methods
     */
    bool m_isCurrentlyLeavingAState;

public:
    IStatemachine();

    /**
     * @brief Statemachine destructor, cleans up states on the stack
     * 
     * 
     * @details
     * Deletes all states that are on the state stack, but before that calls their onLeaveState with NULL_STATE as type argument.
     * 
     * @see NULL_STATE
     */
    virtual ~IStatemachine() noexcept;


    /**
     * @brief Get the state object on top of the state stack or nullptr if SM was not initialized
     * 
     * @return upcasted to StateInterface pointer to the state
     * 
     * 
     * @see init()
     */
    StateInterface *getCurrentState() const noexcept;

    /**
     * @brief Get the type_index of the state object on top of the state stack or NULL_STATE if SM was not initialized
     * 
     * @return std::type_index of the state
     * 
     * 
     * @see init(), NULL_STATE
     */
    std::type_index getCurrentStateType() const noexcept;

    /**
     * @brief Return whether the statemachine is currently in the given state
     * 
     * @tparam StateType type of the state
     * @return if StateType is the type of the current state
     */
    template< class StateType >
    bool isCurrentlyInState() const;

    /**
     * @brief Return the size of statemachine's state stack
     * 
     * @return state stack size
     */
    int getStateStackSize() const noexcept;


    /**
     * @brief Explicitly initialize the statemachine
     * 
     * @tparam StateType type of the initial state
     * @tparam Args types of StateType constructor parameters
     * @param args arguments that should be forwarded to StateType constructor (other than the statemachine pointer)
     * 
     * @throw OnEnterStateException if a state throws exception in a transition method
     * 
     * 
     * @details
     * Use this method to explicitly state that you want to setup the initial state of the statemachine.
     * This initial state will always stay on the bottom of the state stack and won't be able to be popped.
     * If the state stack is already not empty, it won't do anything.
     * onEnterState for the init state is called with NULL_STATE.
     * 
     * StateType is evaluated on compile time to check if it inherits from StateInterface.
     * 
     * If a state throws an exception during onEnterState, the state is still pushed onto the state stack, 
     * but its condition remains undefined.
     * 
     * @see NULL_STATE, OnEnterStateException
     */
    template< class StateType, typename ...Args >
    void init( Args&& ...args );

    /**
     * @brief Transitions directly to specified state, forgetting its previous state afterwards (and deleting it)
     * 
     * @tparam StateType type of the state statemachine should transition to
     * @tparam Args types of StateType constructor parameters
     * @param args arguments that should be forwarded to StateType constructor (other than the statemachine pointer)
     * 
     * @throw OnEnterStateException or OnLeaveStateException if a state throws exception in a transition method
     * 
     * 
     * @details
     * If state stack size is greater than 1, pops the state on the top of state stack and immediately pushes the specified state.
     * State transition happens directly, without transition through the state below the current one on the stack.
     * After the transition, the previous state object is deleted.
     * 
     * If statemachine is currently in the same state as specified, method does nothing.
     * If the state stack is empty or has only init state left - then method acts like pushState().
     * 
     * This method can be used to initialize the statemachine.
     * 
     * StateType is evaluated on compile time to check if it inherits from StateInterface.
     * 
     * If the previous state throws an exception during onLeaveState, the state stack is not updated and the condition of the state
     * which threw the exception remains undefined.
     * If the next state throws an exception during onEnterState, this state is still pushed onto the state stack, 
     * but its condition remains undefined.
     * 
     * @see pushState(), init(), OnEnterStateException, OnLeaveStateException
     */
    template< class StateType, typename ...Args >
    void gotoState( Args&& ...args );

    /**
     * @brief Transitions to the specified state and rememebers its previous state afterwards
     * 
     * @tparam StateType type of the state statemachine should transition to
     * @tparam Args types of StateType constructor parameters
     * @param args arguments that should be forwarded to StateType constructor (other than the statemachine pointer)
     * 
     * @throw OnEnterStateException or OnLeaveStateException if a state throws exception in a transition method
     * 
     * 
     * @details 
     * Pushes next state onto the state stack.
     * State transition goes from state previously on top of the stack to the specified state.
     * If statemachine is currently in the same state as specified, method does nothing.
     * 
     * This method can be used to initialize the statemachine.
     * 
     * StateType is evaluated on compile time to check if it inherits from StateInterface.
     * 
     * If the previous state throws an exception during onLeaveState, the state stack is not updated and the condition of the state
     * which threw the exception remains undefined.
     * If the next state throws an exception during onEnterState, this state is still pushed onto the state stack, 
     * but its condition remains undefined.
     * 
     * @see init()
     */
    template< class StateType, typename ...Args >
    void pushState( Args&& ...args );

    /**
     * @brief Transitions to the previous state.
     * 
     * @throw OnEnterStateException or OnLeaveStateException if a state throws exception in a transition method
     * 
     * 
     * @details 
     * Pops the state on top of the state stack unless there's only one state (the init state) left or none.
     * After the transition, the previous state object is deleted.
     * 
     * If the previous state throws an exception during onLeaveState, the state stack is not updated and the condition of the state
     * which threw the exception remains undefined.
     * If the next state throws an exception during onEnterState, this state still stays on the state stack, 
     * but its condition remains undefined.
     */
    void popState();
};

} // namespace chestnut::fsm


#include "statemachine.inl"


#endif // __CHESTNUT_STATEMACHINE_STATEMACHINE_H__
/**
 * @file statemachine_base.hpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Header file with fsm::StatemachineBase
 * @version 3.0.0
 * @date 2022-01-30
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */


#ifndef __CHESTNUT_STATEMACHINE_STATEMACHINE_BASE_H__
#define __CHESTNUT_STATEMACHINE_STATEMACHINE_BASE_H__

#include "state_base.hpp"
#include "exceptions.hpp"

#include <list>
#include <stack>
#include <typeindex>

namespace chestnut::fsm
{

/**
 * @brief Base statemachine type. This class is used internally.
 */
class StatemachineBase
{
public:
    /**
     * @brief Typedef of the statemachine class this class inherits from - in this case THIS is the base class in itself
     */
    typedef StatemachineBase BaseStatemachineType;

    /**
     * @brief Typedef for the state classes to deduce the class they should inherit from
     */
    typedef StateBase BaseStateType;

    /**
     * @brief Typef of the state extension type. This is the base statemachine type so there is no extension
     */
    typedef void StateExtensionType;


private:
    /**
     * @brief A stack of state pointers
     */
    std::stack< BaseStateType*, std::list<BaseStateType*> > m_stackStates;
    /**
     * @brief A flag set to prevent onLeaveState from calling state change methods
     */
    bool m_isCurrentlyLeavingAState;


public:
    /**
     * @brief Statemachine constructor
     */
    StatemachineBase();

    /**
     * @brief Statemachine destructor, cleans up states on the stack
     * 
     * 
     * @details
     * Deletes all states that are on the state stack, but before that calls their onLeaveState with NULL_STATE as nextState in transition.
     * 
     * @see onLeaveState(), NULL_STATE
     */
    virtual ~StatemachineBase() noexcept;


    /**
     * @brief Get the pointer to a state object on top of the state stack or nullptr if statemachine was not initialized
     * 
     * @return pointer to current state, upcasted to base statemachine state type
     * 
     * 
     * @see initState()
     */
    BaseStateType *getCurrentState() const noexcept;

    /**
     * @brief Get the type_index of the state object on top of the state stack or NULL_STATE if statemachine was not initialized
     * 
     * @return std::type_index of the state
     * 
     * 
     * @see initState(), NULL_STATE
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
     * @return whether statemachine was able to change the state
     * 
     * @throw OnEnterStateException if a state throws exception in a transition method
     * 
     * 
     * @details
     * Use this method to explicitly state that you want to setup the initial state of the statemachine.
     * This initial state will always stay on the bottom of the state stack and won't be able to be popped.
     * If the state stack is already not empty, it won't do anything.
     * onEnterState for the init state is called with NULL_STATE as prevState in transition.
     * canEnterState() is checked in subject state to test if the transition is possible. If it's not then the return value is false.
     * 
     * StateType is evaluated on compile time to check if it inherits from chestnut::fsm::StateBase.
     * 
     * If a state throws an exception during onEnterState, the state is still pushed onto the state stack, 
     * but its condition remains undefined.
     * 
     * @see NULL_STATE, canEnterState(), OnEnterStateException
     */
    template< class StateType, typename ...Args >
    bool initState( Args&& ...args );

    /**
     * @brief Transitions directly to specified state, forgetting its previous state afterwards (and deleting it)
     * 
     * @tparam StateType type of the state statemachine should transition to
     * @tparam Args types of StateType constructor parameters
     * @param args arguments that should be forwarded to StateType constructor (other than the statemachine pointer)
     * 
     * @return whether statemachine was able to change the state
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
     * canLeaveState() and canEnterState() are checked in subject states to test if the transition is possible. If it's not then the return value is false.
     * 
     * This method can be used to initialize the statemachine.
     * 
     * StateType is evaluated on compile time to check if it inherits from chestnut::fsm::StateBase.
     * 
     * If the previous state throws an exception during onLeaveState, the state stack is not updated and the condition of the state
     * which threw the exception remains undefined.
     * If the next state throws an exception during onEnterState, this state is still pushed onto the state stack, 
     * but its condition remains undefined.
     * 
     * @see pushState(), canLeaveState(), canEnterState(), initState(), OnLeaveStateException, OnEnterStateException
     */
    template< class StateType, typename ...Args >
    bool gotoState( Args&& ...args );

    /**
     * @brief Transitions to the specified state and rememebers its previous state afterwards
     * 
     * @tparam StateType type of the state statemachine should transition to
     * @tparam Args types of StateType constructor parameters
     * @param args arguments that should be forwarded to StateType constructor (other than the statemachine pointer)
     * 
     * @return whether statemachine was able to change the state
     * 
     * @throw OnEnterStateException or OnLeaveStateException if a state throws exception in a transition method
     * 
     * 
     * @details 
     * Pushes next state onto the state stack.
     * State transition goes from state previously on top of the stack to the specified state.
     * If statemachine is currently in the same state as specified, method does nothing.
     * canLeaveState() and canEnterState() are checked in subject states to test if the transition is possible. If it's not then the return value is false.
     * 
     * This method can be used to initialize the statemachine.
     * 
     * StateType is evaluated on compile time to check if it inherits from chestnut::fsm::StateBase.
     * 
     * If the previous state throws an exception during onLeaveState, the state stack is not updated and the condition of the state
     * which threw the exception remains undefined.
     * If the next state throws an exception during onEnterState, this state is still pushed onto the state stack, 
     * but its condition remains undefined.
     * 
     * @see canLeaveState(), canEnterState(), initState(), OnLeaveStateException, OnEnterStateException
     */
    template< class StateType, typename ...Args >
    bool pushState( Args&& ...args );

    /**
     * @brief Transitions to the previous state.
     * 
     * @return whether statemachine was able to change the state
     * 
     * @throw OnEnterStateException or OnLeaveStateException if a state throws exception in a transition method
     * 
     * 
     * @details 
     * Pops the state on top of the state stack unless there's only one state (the init state) left or none.
     * After the transition, the previous state object is deleted.
     * canLeaveState() and canEnterState() are checked in subject states to test if the transition is possible. If it's not then the return value is false.
     * 
     * If the previous state throws an exception during onLeaveState, the state stack is not updated and the condition of the state
     * which threw the exception remains undefined.
     * If the next state throws an exception during onEnterState, this state still stays on the state stack, 
     * but its condition remains undefined.
     * 
     * @see canLeaveState(), canEnterState(), OnLeaveStateException, OnEnterStateException
     */
    bool popState();
};

} // namespace chestnut::fsm


#include "statemachine_base.inl"


#endif // __CHESTNUT_STATEMACHINE_STATEMACHINE_BASE_H__
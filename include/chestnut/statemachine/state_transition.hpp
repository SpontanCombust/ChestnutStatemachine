#ifndef __CHESTNUT_STATEMACHINE_STATE_TRANSITION_H__
#define __CHESTNUT_STATEMACHINE_STATE_TRANSITION_H__

#include <typeindex>

namespace chestnut::fsm
{

/**
 * @brief A constant type index meant to represent a lack of state
 * 
 * @details
 * There are currently 3 situations when NULL_STATE is used: \n 
 * 1. When statemachine has not been initialized yet, getCurrentStateType returns NULL_STATE \n
 * 2. When statemachine is being initialized with its init state, then NULL_STATE is passed to onEnterState of that state \n
 * 3. When statemachine is being deleted, then NULL_STATE is passed to onLeaveState of every state on the state stack before deleting them
 */
const std::type_index NULL_STATE = std::type_index( typeid(nullptr) );


/**
 * @brief Enum describing the type of state transition
 */
enum EStateTransitionType
{
    /** When statemachine is being initialized*/
    STATE_TRANSITION_INIT,
    /** When pushState was callled */
    STATE_TRANSITION_PUSH,
    /** When gotoState was callled */
    STATE_TRANSITION_GOTO,
    /** When popState was callled */
    STATE_TRANSITION_POP,
    /** When statemachine is being destroyed */
    STATE_TRANSITION_DESTROY
};


/**
 * @brief Struct containing data describing a state transition
 */
struct StateTransition
{
    EStateTransitionType type;
    std::type_index prevState = NULL_STATE;
    std::type_index nextState = NULL_STATE; 
};

} // namespace chestnut::fsm

#endif // __CHESTNUT_STATEMACHINE_STATE_TRANSITION_H__
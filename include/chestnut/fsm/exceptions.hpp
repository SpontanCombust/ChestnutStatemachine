/**
 * @file exceptions.hpp
 * @author SpontanCombust
 * @brief Header file with exception types.
 * @version 3.0.0
 * @date 2022-01-20
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */


#ifndef __CHESTNUT_STATEMACHINE_EXCEPTIONS_H__
#define __CHESTNUT_STATEMACHINE_EXCEPTIONS_H__

#include "state_transition.hpp"

#include <exception>
#include <string>

namespace chestnut::fsm
{

/**
 * @brief Base exception class for all statemachine related exceptions
 */
struct StatemachineException : std::exception
{
    std::string message;

    StatemachineException( const char *msg ) throw();
    const char* what() const throw() override;
};

struct BadParentAccessException : StatemachineException
{
    BadParentAccessException( const char *msg );
};

/**
 * @brief Base exception class for OnEnterStateException and OnLeaveStateException
 * 
 * @see OnEnterStateException
 * @see OnLeaveStateException
 */
struct StateChangeException : StatemachineException
{
    StateTransition transition;

    StateChangeException( StateTransition transition, const char *msg ) throw();
};

/**
 * @brief Exception type thrown if onEnterState method in a state throws an unhandled exception
 */
struct OnEnterStateException : StateChangeException
{
    OnEnterStateException( StateTransition transition, const char *msg ) throw();
};

/**
 * @brief Exception type thrown if onLeaveState method in a state throws an unhandled exception
 */
struct OnLeaveStateException : StateChangeException
{
    OnLeaveStateException( StateTransition transition, const char *msg ) throw();
};

} // namespace chestnut::fsm


#include "exceptions.inl"


#endif // __CHESTNUT_STATEMACHINE_EXCEPTIONS_H__
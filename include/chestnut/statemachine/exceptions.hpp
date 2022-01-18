/**
 * @file exceptions.hpp
 * @author SpontanCombust
 * @brief Header file with exception types.
 * @version 2.1.0
 * @date 2021-11-28
 * 
 * @copyright MIT License (c) 2021
 * 
 */


#ifndef __CHESTNUT_STATEMACHINE_EXCEPTIONS_H__
#define __CHESTNUT_STATEMACHINE_EXCEPTIONS_H__

#include <exception>
#include <string>

namespace chestnut::fsm
{

/**
 * @brief Base exception class for OnEnterException and OnExitException
 * 
 * @see OnEnterException
 * @see OnExitException
 */
struct StateChangeException : std::exception
{
    std::string message;

    StateChangeException( const char *msg ) throw();
    const char* what() const throw() override;
};

/**
 * @brief Exception type thrown if onEnter method in a state throws an unhandled exception
 */
struct OnEnterException : StateChangeException
{
    OnEnterException( const char *msg ) throw();
};

/**
 * @brief Exception type thrown if onExit method in a state throws an unhandled exception
 */
struct OnExitException : StateChangeException
{
    OnExitException( const char *msg ) throw();
};

} // namespace chestnut::fsm


#include "exceptions.inl"


#endif // __CHESTNUT_STATEMACHINE_EXCEPTIONS_H__
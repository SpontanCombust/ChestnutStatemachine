#ifndef __CHESTNUT_STATEMACHINE_EXCEPTIONS_H__
#define __CHESTNUT_STATEMACHINE_EXCEPTIONS_H__

#include <exception>
#include <string>

namespace chestnut::statemachine
{
    struct StateChangeException : std::exception
    {
        std::string message;

        StateChangeException( const char *msg ) throw();
        const char* what() const throw() override;
    };

    struct OnEnterException : StateChangeException
    {
        OnEnterException( const char *msg ) throw();
    };

    struct OnExitException : StateChangeException
    {
        OnExitException( const char *msg ) throw();
    };

} // namespace chestnut::statemachine


#include "exceptions.inl"


#endif // __CHESTNUT_STATEMACHINE_EXCEPTIONS_H__
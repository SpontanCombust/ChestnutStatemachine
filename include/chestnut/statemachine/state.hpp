#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

#include <typeindex>

namespace chestnut::statemachine
{
    template< class ParentStatemachine >
    class IState
    {
    protected:
        ParentStatemachine *parent;

    public:
        typedef ParentStatemachine* ParentStatemachinePtrType;

    public:
        IState( ParentStatemachine *parent_ );
        virtual ~IState() = default;
        
        virtual void onEnter( std::type_index prevState ) = 0;
        virtual void onExit( std::type_index nextState ) = 0;
    };
    
} // namespace chestnut::statemachine


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
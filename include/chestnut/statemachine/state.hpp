#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

namespace chestnut::statemachine
{
    template< typename StateNameType, class ParentStatemachine >
    class IState
    {
    protected:
        ParentStatemachine *parent;

    public:
        typedef ParentStatemachine* ParentStatemachinePtrType;
        StateNameType name; // has to be set by inheriting state type

    public:
        IState( ParentStatemachine *parent_ );
        virtual ~IState() = default;
        
        virtual void onEnter( StateNameType prevState ) = 0;
        virtual void onExit( StateNameType nextState ) = 0;
    };
    
} // namespace chestnut::statemachine


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
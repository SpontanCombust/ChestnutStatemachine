#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

namespace chestnut::statemachine
{
    template< typename StateNameType, class ParentStatemachine >
    class IState
    {
    public:
        typedef ParentStatemachine* ParentStatemachinePtrType;

    protected:
        ParentStatemachine *parent;

    public:
        IState( ParentStatemachine *parent_ );
        virtual ~IState() = default;

        virtual const StateNameType getName() const = 0;
        virtual void onEnter( StateNameType prevState ) = 0;
        virtual void onExit( StateNameType nextState ) = 0;
    };
    
} // namespace chestnut::statemachine


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
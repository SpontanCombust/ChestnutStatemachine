#ifndef __CHESTNUT_STATEMACHINE_STATEMACHINE_H__
#define __CHESTNUT_STATEMACHINE_STATEMACHINE_H__

#include <stack>
#include <typeindex>
#include <unordered_map>

namespace chestnut::statemachine
{
    // StateInterface must inherit from IState
    template< class StateInterface >
    class IStatemachine
    {
    protected:
        std::unordered_map< std::type_index, StateInterface* > m_mapStateTypeToState;
        std::stack< StateInterface* > m_stackStates;

    public:
        IStatemachine() = default;

        virtual ~IStatemachine();


        // Adds states of given types to the statemachine
        // States must inherit from StateInterface type
        // DefaultState type is the state type the statemachine is initialized with and will always stay on the bottom of state stack
        // onEnter for the default state is called with the name of the default state
        //
        // It is possible to pass in only default state type
        // This method should be called only once and always before calling any other method
        template< class DefaultState, class ...States >
        void setupStates();


        virtual StateInterface *getCurrentState() const;

        virtual std::type_index getCurrentStateType() const;


        // If state stack size is greater than 1, pops the state on the top of state stack and immediately pushes the specified state
        // State transition happens directly, without consideration of a state below the top
        //
        // If state stack has only 1 element (the default state) the method acts like pushState()
        // If state with that name was not supplied in the constructor or the same state is on top of the stack, method does nothing
        virtual void gotoState( std::type_index nextStateType );

        // Pushes next state onto the state stack
        // State transition goes from state previously on top of the stack to the specified state
        //
        // If state with that name was not supplied in the constructor or the same state is on top of the stack, method does nothing
        virtual void pushState( std::type_index nextStateType );

        // Pops the state on top of the state stack unless there's only one state left (the default state)
        virtual void popState();


    private:
        void destroy();


        template< class ...SystemTypes >
        struct SystemTypeList {};

        void addStates( SystemTypeList<> );

        template< class State, class ...OtherStates >
        void addStates( SystemTypeList<State, OtherStates...> );
    };
    
} // namespace chestnut::statemachine


#include "statemachine.inl"


#endif // __CHESTNUT_STATEMACHINE_STATEMACHINE_H__
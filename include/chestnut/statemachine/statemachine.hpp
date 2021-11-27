#ifndef __CHESTNUT_STATEMACHINE_STATEMACHINE_H__
#define __CHESTNUT_STATEMACHINE_STATEMACHINE_H__

#include <stack>
#include <typeindex>
#include <unordered_map>

namespace chestnut::statemachine
{
    /**
     * @brief Base generic class for creating a statemachine
     * 
     * @tparam StateInterface a base (interface) class from which all states inherit
     * 
     * @details
     * StateInterface type must inherit from IState to work properly.
     * It can have any number of virtual methods that proper states should implement in some form
     * 
     * @see IState
     */
    template< typename StateInterface >
    class IStatemachine
    {
    protected:
        std::unordered_map< std::type_index, StateInterface* > m_mapStateTypeToState;
        std::stack< StateInterface* > m_stackStates;

    public:
        IStatemachine() = default;

        virtual ~IStatemachine();


        /**
         * @brief Adds states of given types to the statemachine and initialize the default state
         * 
         * @details
         * States must inherit from StateInterface type.
         * DefaultState type is the state type the statemachine is initialized with and will always stay on the bottom of state stack.
         * When finished, method calls onEnter for the default state with its own name
         * It is possible to pass in only default state type.
         * This method should be called only once and always before calling any other method.
         * 
         * @tparam DefaultState type of the default state for the statemachine
         * @tparam States rest of the states
         */
        template< class DefaultState, class ...States >
        void setupStates();

        /**
         * @brief Get the state object on top of the state stack
         * 
         * @return StateInterface* upcasted pointer to the state
         */
        virtual StateInterface *getCurrentState() const;

        /**
         * @brief Get the type_index of the state object on top of the state stack
         * 
         * @return std::type_index of the state
         */
        virtual std::type_index getCurrentStateType() const;


        /**
         * @brief Transitions directly to specified state, forgetting its previous state afterwards
         * 
         * @details
         * If state stack size is greater than 1, pops the state on the top of state stack and immediately pushes the specified state.
         * State transition happens directly, without consideration of a state below the top.
         * If state stack has only 1 element (the default state) the method acts like pushState().
         * If state of that type was not supplied in the constructor or the same state is on top of the stack, method does nothing.
         * 
         * @param nextState type of the state statemachine should transition to
         */
        virtual void gotoState( std::type_index nextState );

        /**
         * @brief Transitions to the specified state and rememebers its previous state afterwards
         * 
         * @details 
         * Pushes next state onto the state stack.
         * State transition goes from state previously on top of the stack to the specified state.
         * If state of that type was not supplied in the constructor or the same state is on top of the stack, method does nothing.
         * 
         * @param nextState type of the state statemachine should transition to
         */
        virtual void pushState( std::type_index nextState );

        /**
         * @brief Transitions to the previous state
         * 
         * @details 
         * Pops the state on top of the state stack unless there's only one state left (the default state).
         */
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
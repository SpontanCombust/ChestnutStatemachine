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
        std::stack< StateInterface* > m_stackStates;

    public:
        IStatemachine() = default;

        /**
         * @brief Statemachine destroyer, cleans up states on the stack
         * 
         * @details
         * Deletes all states that are on the state stack, but before that calls their onExit with NULL_STATE as type argument.
         * @see NULL_STATE
         */
        virtual ~IStatemachine();


        /**
         * @brief Get the state object on top of the state stack or nullptr if SM was not initialized
         * 
         * @see init()
         * 
         * @return StateInterface* upcasted pointer to the state
         */
        StateInterface *getCurrentState() const;

        /**
         * @brief Get the type_index of the state object on top of the state stack or NULL_STATE if SM was not initialized
         * 
         * @see init()
         * @see NULL_STATE
         * 
         * @return std::type_index of the state
         */
        std::type_index getCurrentStateType() const;


        /**
         * @brief Explicitly initialize the statemachine
         * 
         * @details
         * Use this method to explicitly state that you want to setup the initial state of the statemachine.
         * This initial state will always stay on the bottom of the state stack and won't be able to be popped.
         * If the state stack is already not empty, it won't do anything.
         * onEnter for the init state is called with NULL_STATE
         * @see NULL_STATE
         * 
         * StateType is evaluated on compile time to check if it inherits from StateInterface
         * 
         * @tparam StateType type of the initial state
         */
        template< class StateType >
        void init();

        /**
         * @brief Transitions directly to specified state, forgetting its previous state afterwards (and deleting it)
         * 
         * @details
         * If state stack size is greater than 1, pops the state on the top of state stack and immediately pushes the specified state.
         * State transition happens directly, without transition through the state below the current one on the stack.
         * After the transition, the previous state object is deleted.
         * 
         * If statemachine is currently in the same state as specified, method does nothing.
         * If the state stack is empty or has only init state left - then method acts like pushState().
         * @see pushState()
         * 
         * This method can be used to initialize the statemachine.
         * @see init()
         * 
         * StateType is evaluated on compile time to check if it inherits from StateInterface.
         * 
         * @tparam StateType type of the state statemachine should transition to
         */
        template< class StateType >
        void gotoState();

        /**
         * @brief Transitions to the specified state and rememebers its previous state afterwards
         * 
         * @details 
         * Pushes next state onto the state stack.
         * State transition goes from state previously on top of the stack to the specified state.
         * If statemachine is currently in the same state as specified, method does nothing.
         * 
         * This method can be used to initialize the statemachine.
         * @see init()
         * 
         * StateType is evaluated on compile time to check if it inherits from StateInterface.
         * 
         * @tparam StateType type of the state statemachine should transition to
         */
        template< class StateType >
        void pushState();

        /**
         * @brief Transitions to the previous state.
         * 
         * @details 
         * Pops the state on top of the state stack unless there's only one state (the init state) left or none.
         * After the transition, the previous state object is deleted.
         */
        void popState();
    };
    
} // namespace chestnut::statemachine


#include "statemachine.inl"


#endif // __CHESTNUT_STATEMACHINE_STATEMACHINE_H__
#include <type_traits>

namespace chestnut::statemachine
{  
    template<class StateInterface>
    IStatemachine<StateInterface>::~IStatemachine() 
    {
        while( !m_stackStates.empty() )
        {
            StateInterface *state = m_stackStates.top();
            state->onExit( NULL_STATE );
            delete state;
            m_stackStates.pop();
        }
    }

    template<class StateInterface>
    StateInterface* IStatemachine<StateInterface>::getCurrentState() const
    {
        if( m_stackStates.size() >= 1 )
        {
            return m_stackStates.top();
        }

        return nullptr;
    }

    template<class StateInterface>
    std::type_index IStatemachine<StateInterface>::getCurrentStateType() const
    {
        if( m_stackStates.size() >= 1 )
        {
            return std::type_index( typeid( *m_stackStates.top() ) );
        }

        return NULL_STATE;
    }

    //TODO guard against possible exceptions from onEnter and onExit
    template<typename StateInterface>
    template<class StateType>
    void IStatemachine<StateInterface>::init() 
    {
        static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

        if( m_stackStates.empty() )
        {
            StateInterface *initState = new StateType( dynamic_cast<typename StateInterface::ParentStatemachinePtrType>( this ) );
            m_stackStates.push( initState );
            initState->onEnter( NULL_STATE );
        }
    }

    //TODO guard against possible exceptions from onEnter and onExit
    template<typename StateInterface>
    template<class StateType>
    void IStatemachine<StateInterface>::gotoState() 
    {
        static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

        std::type_index currentStateType = getCurrentStateType();
        std::type_index nextStateType = typeid(StateType);

        if( nextStateType != currentStateType )
        {
            StateInterface *nextState = new StateType( dynamic_cast<typename StateInterface::ParentStatemachinePtrType>( this ) );

            if( m_stackStates.size() >= 1 )
            {
                StateInterface *currentState = m_stackStates.top();

                currentState->onExit( nextStateType );

                // we want to always retain the init state on the stack
                if( m_stackStates.size() > 1 )
                {
                    // we transition to next state directly and forget the previous state
                    delete currentState;
                    m_stackStates.pop();
                }

                m_stackStates.push( nextState );

                nextState->onEnter( currentStateType );
            }
            else
            {
                m_stackStates.push( nextState );
                nextState->onEnter( NULL_STATE );
            }
        }
    }

    //TODO guard against possible exceptions from onEnter and onExit
    template<typename StateInterface>
    template<class StateType>
    void IStatemachine<StateInterface>::pushState() 
    {
        static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

        std::type_index currentStateType = getCurrentStateType();
        std::type_index nextStateType = typeid(StateType);

        if( nextStateType != currentStateType )
        {
            StateInterface *nextState = new StateType( dynamic_cast<typename StateInterface::ParentStatemachinePtrType>( this ) );

            if( m_stackStates.size() >= 1 )
            {
                StateInterface *currentState = m_stackStates.top();

                currentState->onExit( nextStateType );

                // with pushState we remember the previous state, so we don't delete it and leave it on state stack

                m_stackStates.push( nextState );

                nextState->onEnter( currentStateType );
            }
            else
            {
                m_stackStates.push( nextState );
                nextState->onEnter( NULL_STATE );
            }
        }
    }

    //TODO guard against possible exceptions from onEnter and onExit
    template<class StateInterface>
    void IStatemachine<StateInterface>::popState() 
    {
        // we want to always retain the init state on the stack
        if( m_stackStates.size() > 1 )
        {
            StateInterface *currentState = m_stackStates.top();

            m_stackStates.pop();

            StateInterface *nextState = m_stackStates.top();

            currentState->onExit( typeid( *nextState ) );
            nextState->onEnter( typeid( *currentState ) );

            delete currentState;
        }
    }

} // namespace chestnut::statemachine

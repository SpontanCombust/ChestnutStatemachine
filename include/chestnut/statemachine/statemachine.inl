namespace chestnut::statemachine
{  
    template<typename StateNameType, typename StateInterface>
    template<class DefaultState, class ...States>
    void IStatemachine<StateNameType, StateInterface>::setupStates() 
    {
        destroy();

        DefaultState *defaultState = new DefaultState( dynamic_cast<typename DefaultState::ParentStatemachinePtrType>( this ) );

        m_mapStateNameToState[ defaultState->name ] = defaultState;
        m_stackStates.push( defaultState );

        addStates( SystemTypeList<States...>() );

        defaultState->onEnter( defaultState->name );
    }

    template<typename StateNameType, typename StateInterface>
    void IStatemachine<StateNameType, StateInterface>::addStates( SystemTypeList<> ) 
    {
        // NOP
    }

    template<typename StateNameType, typename StateInterface>
    template<class State, class ...OtherStates>
    void IStatemachine<StateNameType, StateInterface>::addStates( SystemTypeList<State, OtherStates...> ) 
    {
        State *state = new State( dynamic_cast<typename State::ParentStatemachinePtrType>( this ) );

        auto it = m_mapStateNameToState.find( state->name );
        // add the state only if it's not a duplicate
        if( it == m_mapStateNameToState.end() )
        {
            m_mapStateNameToState[ state->name ] = state;
        }
        else
        {
            delete state;
        }

        addStates( SystemTypeList<OtherStates...>() );
    }

    template<typename StateNameType, typename StateInterface>
    void IStatemachine<StateNameType, StateInterface>::destroy() 
    {
        for( auto it = m_mapStateNameToState.begin(); it != m_mapStateNameToState.end(); ++it )
        {
            delete it->second;
        }

        m_mapStateNameToState.clear();

        while( !m_stackStates.empty() )
        {
            m_stackStates.pop();
        }
    }

    template<typename StateNameType, typename StateInterface>
    IStatemachine<StateNameType, StateInterface>::~IStatemachine() 
    {
        destroy();
    }

    template<typename StateNameType, typename StateInterface>
    StateInterface* IStatemachine<StateNameType, StateInterface>::getCurrentState() const
    {
        return m_stackStates.top();
    }

    template<typename StateNameType, typename StateInterface>
    StateNameType IStatemachine<StateNameType, StateInterface>::getCurrentStateName() const
    {
        return m_stackStates.top()->name;   
    }

    template<typename StateNameType, typename StateInterface>
    void IStatemachine<StateNameType, StateInterface>::gotoState( StateNameType nextStateName ) 
    {
        auto it = m_mapStateNameToState.find( nextStateName );
        if( it != m_mapStateNameToState.end() )
        {
            StateInterface *currentState = m_stackStates.top();
            StateNameType currentStateName = currentState->name;

            if( currentStateName != nextStateName )
            {
                currentState->onExit( nextStateName );
            
                currentState = it->second; // here it becomes a next state
                // we want to always retain at least one state on the stack - the default one
                if( m_stackStates.size() > 1 )
                {
                    m_stackStates.pop();
                }
                m_stackStates.push( currentState );

                currentState->onEnter( currentStateName );
            }
        }
    }

    template<typename StateNameType, typename StateInterface>
    void IStatemachine<StateNameType, StateInterface>::pushState( StateNameType nextStateName ) 
    {
        auto it = m_mapStateNameToState.find( nextStateName );
        if( it != m_mapStateNameToState.end() )
        {
            StateInterface *currentState = m_stackStates.top();
            StateNameType currentStateName = currentState->name;

            if( currentStateName != nextStateName )
            {
                currentState->onExit( nextStateName );
            
                currentState = it->second; // here it becomes a next state
                m_stackStates.push( currentState );

                currentState->onEnter( currentStateName );
            }
        }
    }

    template<typename StateNameType, typename StateInterface>
    void IStatemachine<StateNameType, StateInterface>::popState() 
    {
        // we want to always retain at least one state on the stack - the default one
        if( m_stackStates.size() > 1 )
        {
            StateInterface *currentState = m_stackStates.top();

            m_stackStates.pop();

            StateInterface *nextState = m_stackStates.top();

            currentState->onExit( nextState->name );
            nextState->onEnter( currentState->name );
        }
    }

} // namespace chestnut::statemachine

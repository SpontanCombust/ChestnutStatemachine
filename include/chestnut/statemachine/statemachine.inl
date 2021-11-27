namespace chestnut::statemachine
{  
    template<class StateInterface>
    template<class DefaultState, class ...States>
    void IStatemachine<StateInterface>::setupStates() 
    {
        destroy();

        DefaultState *defaultState = new DefaultState( dynamic_cast<typename DefaultState::ParentStatemachinePtrType>( this ) );

        m_mapStateTypeToState[ typeid(DefaultState) ] = defaultState;
        m_stackStates.push( defaultState );

        addStates( SystemTypeList<States...>() );

        defaultState->onEnter( typeid(DefaultState) );
    }

    template<class StateInterface>
    void IStatemachine<StateInterface>::addStates( SystemTypeList<> ) 
    {
        // NOP
    }

    template<class StateInterface>
    template<class State, class ...OtherStates>
    void IStatemachine<StateInterface>::addStates( SystemTypeList<State, OtherStates...> ) 
    {
        auto it = m_mapStateTypeToState.find( typeid(State) );
        // add the state only if it's not a duplicate
        if( it == m_mapStateTypeToState.end() )
        {
            m_mapStateTypeToState[ typeid(State) ] = new State( dynamic_cast<typename State::ParentStatemachinePtrType>( this ) );
        }

        addStates( SystemTypeList<OtherStates...>() );
    }

    template<class StateInterface>
    void IStatemachine<StateInterface>::destroy() 
    {
        for( auto it = m_mapStateTypeToState.begin(); it != m_mapStateTypeToState.end(); ++it )
        {
            delete it->second;
        }

        m_mapStateTypeToState.clear();

        while( !m_stackStates.empty() )
        {
            m_stackStates.pop();
        }
    }

    template<class StateInterface>
    IStatemachine<StateInterface>::~IStatemachine() 
    {
        destroy();
    }

    template<class StateInterface>
    StateInterface* IStatemachine<StateInterface>::getCurrentState() const
    {
        return m_stackStates.top();
    }

    template<class StateInterface>
    std::type_index IStatemachine<StateInterface>::getCurrentStateType() const
    {
        return std::type_index( typeid( *m_stackStates.top() ) );
    }

    template<class StateInterface>
    void IStatemachine<StateInterface>::gotoState( std::type_index nextStateType ) 
    {
        auto it = m_mapStateTypeToState.find( nextStateType );
        if( it != m_mapStateTypeToState.end() )
        {
            StateInterface *currentState = m_stackStates.top();
            std::type_index currentStateType = typeid(*currentState);

            if( currentStateType != nextStateType )
            {
                currentState->onExit( nextStateType );
            
                currentState = it->second; // here it becomes a next state
                // we want to always retain at least one state on the stack - the default one
                if( m_stackStates.size() > 1 )
                {
                    m_stackStates.pop();
                }
                m_stackStates.push( currentState );

                currentState->onEnter( currentStateType );
            }
        }
    }

    template<class StateInterface>
    void IStatemachine<StateInterface>::pushState( std::type_index nextStateType ) 
    {
        auto it = m_mapStateTypeToState.find( nextStateType );
        if( it != m_mapStateTypeToState.end() )
        {
            StateInterface *currentState = m_stackStates.top();
            std::type_index currentStateType = typeid(*currentState);

            if( currentStateType != nextStateType )
            {
                currentState->onExit( nextStateType );
            
                currentState = it->second; // here it becomes a next state
                m_stackStates.push( currentState );

                currentState->onEnter( currentStateType );
            }
        }
    }

    template<class StateInterface>
    void IStatemachine<StateInterface>::popState() 
    {
        // we want to always retain at least one state on the stack - the default one
        if( m_stackStates.size() > 1 )
        {
            StateInterface *currentState = m_stackStates.top();

            m_stackStates.pop();

            StateInterface *nextState = m_stackStates.top();

            currentState->onExit( typeid(*nextState) );
            nextState->onEnter( typeid(*currentState) );
        }
    }

} // namespace chestnut::statemachine

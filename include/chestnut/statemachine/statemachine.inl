#include <type_traits>
#include <cstdio>

namespace chestnut::fsm
{  

template<typename StateInterface>
IStatemachine<StateInterface>::IStatemachine() 
{
    // basically check on compile time if the StateInterface can be a valid state type for this machine
    static_assert( std::is_base_of< IStatemachine<StateInterface>, typename StateInterface::ParentStatemachineType >::value, "StateInterface is not a valid state class for this statemachine!" );

    m_isCurrentlyLeavingAState = false;
}

template<class StateInterface>
IStatemachine<StateInterface>::~IStatemachine() noexcept
{
    m_isCurrentlyLeavingAState = true;

    StateTransition transition;
    transition.type = STATE_TRANSITION_DESTROY;
    transition.nextState = NULL_STATE;

    while( !m_stackStates.empty() )
    {
        StateInterface *state = m_stackStates.top();
        m_stackStates.pop();

        transition.prevState = typeid( *state );

        try
        {
            state->onLeaveState( transition );
        }
        catch(const std::exception& e)
        {
            fprintf( stderr, "%s\n", e.what() );
        }
        
        delete state;
    }
}

template<class StateInterface>
StateInterface* IStatemachine<StateInterface>::getCurrentState() const noexcept
{
    if( m_stackStates.size() >= 1 )
    {
        return m_stackStates.top();
    }

    return nullptr;
}

template<class StateInterface>
std::type_index IStatemachine<StateInterface>::getCurrentStateType() const noexcept
{
    if( m_stackStates.size() >= 1 )
    {
        return std::type_index( typeid( *m_stackStates.top() ) );
    }

    return NULL_STATE;
}

template<typename StateInterface>
template<class StateType>
bool IStatemachine<StateInterface>::isCurrentlyInState() const
{
    if( m_stackStates.size() >= 1 )
    {
        return std::type_index( typeid( *m_stackStates.top() ) ) == std::type_index( typeid( StateType ) );
    }

    return false;
}

template<typename StateInterface>
int IStatemachine<StateInterface>::getStateStackSize() const noexcept
{
    return (int)m_stackStates.size();
}

template<typename StateInterface>
template<class StateType, typename ...Args>
bool IStatemachine<StateInterface>::init( Args&& ...args ) 
{
    static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

    if( !m_stackStates.empty() )
    {
		return false;
    }

	StateTransition transition;
	transition.type = STATE_TRANSITION_INIT;
	transition.prevState = NULL_STATE;
	transition.nextState = typeid( StateType );

	StateInterface *initState = new StateType( std::forward<Args>(args)... );
    initState->setParent( static_cast<typename StateInterface::ParentStatemachinePtrType>( this ) );

	if( initState->canEnterState( transition ) )
	{
		m_stackStates.push( initState );

		try
		{
			initState->onEnterState( transition );
		}
		catch(const std::exception& e)
		{
			throw OnEnterStateException( e.what() );   
		}

		return true;
	}
	else
	{
		// polymorphism is needed to check the condition, so this akward immediate deletion after failure is necessary unfortunatelly
		delete initState;
		return false;
	}
}

template<typename StateInterface>
template<class StateType, typename ...Args>
bool IStatemachine<StateInterface>::gotoState( Args&& ...args ) 
{
    static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

    if( m_isCurrentlyLeavingAState )
    {
        return false;
    }

	StateTransition transition;
	transition.prevState = getCurrentStateType();
	transition.nextState = typeid( StateType );
	
    if( transition.prevState != transition.nextState )
    {
        StateInterface *nextState = new StateType( std::forward<Args>(args)... );
        nextState->setParent( static_cast<typename StateInterface::ParentStatemachinePtrType>( this ) );

		transition.type = ( m_stackStates.size() >= 1 ) ? STATE_TRANSITION_GOTO : STATE_TRANSITION_INIT;

		if( !nextState->canEnterState( transition ) )
		{
			delete nextState;
			return false;
		}

        if( transition.type == STATE_TRANSITION_GOTO )
        {
            StateInterface *currentState = m_stackStates.top();

			if( !currentState->canLeaveState( transition ) )
			{
				delete nextState;
				return false;
			}


            m_isCurrentlyLeavingAState = true;

            try
            {
                currentState->onLeaveState( transition );
            }
            catch(const std::exception& e)
            {
                m_isCurrentlyLeavingAState = false;
                delete nextState;
                throw OnLeaveStateException( e.what() );    
            }

			// if not only the init state is on the stack
            if( m_stackStates.size() > 1 ) 
            {
                m_stackStates.pop();
                delete currentState;
            }

            m_isCurrentlyLeavingAState = false;
        }

		m_stackStates.push( nextState );

		try
		{
			nextState->onEnterState( transition );
		}
		catch(const std::exception& e)
		{
			throw OnEnterStateException( e.what() );
		}

		return true;
    }

    return false;
}

template<typename StateInterface>
template<class StateType, typename ...Args>
bool IStatemachine<StateInterface>::pushState( Args&& ...args ) 
{
    static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

    if( m_isCurrentlyLeavingAState )
    {
        return false;
    }

    StateTransition transition;
	transition.prevState = getCurrentStateType();
	transition.nextState = typeid( StateType );
	
    if( transition.prevState != transition.nextState )
    {
        StateInterface *nextState = new StateType( std::forward<Args>(args)... );
        nextState->setParent( static_cast<typename StateInterface::ParentStatemachinePtrType>( this ) );

		transition.type = ( m_stackStates.size() >= 1 ) ? STATE_TRANSITION_PUSH : STATE_TRANSITION_INIT;

		if( !nextState->canEnterState( transition ) )
		{
			delete nextState;
			return false;
		}

        if( transition.type == STATE_TRANSITION_PUSH )
        {
            StateInterface *currentState = m_stackStates.top();

			if( !currentState->canLeaveState( transition ) )
			{
				delete nextState;
				return false;
			}


            m_isCurrentlyLeavingAState = true;

            try
            {
                currentState->onLeaveState( transition );
            }
            catch(const std::exception& e)
            {
                m_isCurrentlyLeavingAState = false;
                delete nextState;
                throw OnLeaveStateException( e.what() );    
            }

            m_isCurrentlyLeavingAState = false;
        }

		m_stackStates.push( nextState );

		try
		{
			nextState->onEnterState( transition );
		}
		catch(const std::exception& e)
		{
			throw OnEnterStateException( e.what() );
		}

		return true;
    }

    return false;
}

template<class StateInterface>
bool IStatemachine<StateInterface>::popState() 
{
    if( m_isCurrentlyLeavingAState )
    {
        return false;
    }

    // we want to always retain the init state on the stack
    if( m_stackStates.size() > 1 )
    {
        StateInterface *currentState = m_stackStates.top();
        std::type_index currentStateType = typeid( *currentState );

        m_stackStates.pop();

        StateInterface *nextState = m_stackStates.top();
        std::type_index nextStateType = typeid( *nextState );

        StateTransition transition;
        transition.type = STATE_TRANSITION_POP;
        transition.prevState = currentStateType;
        transition.nextState = nextStateType;

		if( !currentState->canLeaveState( transition ) || !nextState->canEnterState( transition ) )
		{
			// recover state
			m_stackStates.push( currentState );
			return false;
		}
		
		
        m_isCurrentlyLeavingAState = true;

        try
        {
            currentState->onLeaveState( transition );
        }
        catch(const std::exception& e)
        {
            m_isCurrentlyLeavingAState = false;
            // push this state back so that SM goes back to as it was before except now its condition is undefined
            m_stackStates.push( currentState );
            throw OnLeaveStateException( e.what() );
        }
        
        delete currentState;

        m_isCurrentlyLeavingAState = false;


        try
        {
            nextState->onEnterState( transition );
        }
        catch(const std::exception& e)
        {
            throw OnEnterStateException( e.what() );
        }
    }

	return false;
}

} // namespace chestnut::fsm

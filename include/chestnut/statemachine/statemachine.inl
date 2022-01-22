#include <type_traits>
#include <cstdio>

namespace chestnut::fsm
{  

template<class BaseStateClass>
Statemachine<BaseStateClass>::Statemachine() 
{
    static_assert( std::is_base_of< fsm::StateBase, BaseStateClass >::value, "BaseStateClass is not a valid class for this statemachine! It does not iherit from fsm::StateBase!" );

    m_isCurrentlyLeavingAState = false;
}

template<class BaseStateClass>
Statemachine<BaseStateClass>::~Statemachine() noexcept
{
    m_isCurrentlyLeavingAState = true;

    StateTransition transition;
    transition.type = STATE_TRANSITION_DESTROY;
    transition.nextState = NULL_STATE;

    while( !m_stackStates.empty() )
    {
        BaseStateClass *state = m_stackStates.top();
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

template<class BaseStateClass>
BaseStateClass* Statemachine<BaseStateClass>::getCurrentState() const noexcept
{
    if( m_stackStates.size() >= 1 )
    {
        return m_stackStates.top();
    }

    return nullptr;
}

template<class BaseStateClass>
std::type_index Statemachine<BaseStateClass>::getCurrentStateType() const noexcept
{
    if( m_stackStates.size() >= 1 )
    {
        return std::type_index( typeid( *m_stackStates.top() ) );
    }

    return NULL_STATE;
}

template<class BaseStateClass>
template<class StateType>
bool Statemachine<BaseStateClass>::isCurrentlyInState() const
{
    if( m_stackStates.size() >= 1 )
    {
        return std::type_index( typeid( *m_stackStates.top() ) ) == std::type_index( typeid( StateType ) );
    }

    return false;
}

template<class BaseStateClass>
int Statemachine<BaseStateClass>::getStateStackSize() const noexcept
{
    return (int)m_stackStates.size();
}

template<class BaseStateClass>
template<class StateType, typename ...Args>
bool Statemachine<BaseStateClass>::initState( Args&& ...args ) 
{
    static_assert( std::is_base_of<BaseStateClass, StateType>::value, 
                   "This StateType can't be used with this statemachine! It inherits from a different base state class than that of statemachine's!" );
    static_assert( std::is_base_of< Statemachine<BaseStateClass>, class StateType::StatemachineType>::value, 
                   "This StateType can't be used with this statemachine! It expects a different statemachine class!" );


    if( !m_stackStates.empty() )
    {
		return false;
    }

	StateTransition transition;
	transition.type = STATE_TRANSITION_INIT;
	transition.prevState = NULL_STATE;
	transition.nextState = typeid( StateType );

	StateType *initialState = new StateType( std::forward<Args>(args)... );
    initialState->setParent( static_cast<typename StateType::StatemachinePtrType>( this ) );

	if( initialState->canEnterState( transition ) )
	{
		m_stackStates.push( initialState );

		try
		{
			initialState->onEnterState( transition );
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
		delete initialState;
		return false;
	}
}

template<class BaseStateClass>
template<class StateType, typename ...Args>
bool Statemachine<BaseStateClass>::gotoState( Args&& ...args ) 
{
    static_assert( std::is_base_of<BaseStateClass, StateType>::value, 
                   "This StateType can't be used with this statemachine! It inherits from a different base state class than that of statemachine's!" );
    static_assert( std::is_base_of< Statemachine<BaseStateClass>, class StateType::StatemachineType>::value, 
                   "This StateType can't be used with this statemachine! It expects a different statemachine class!" );


    if( m_isCurrentlyLeavingAState )
    {
        return false;
    }

	StateTransition transition;
	transition.prevState = getCurrentStateType();
	transition.nextState = typeid( StateType );
	
    if( transition.prevState != transition.nextState )
    {
        StateType *nextState = new StateType( std::forward<Args>(args)... );
        nextState->setParent( static_cast<typename StateType::StatemachinePtrType>( this ) );

		transition.type = ( m_stackStates.size() >= 1 ) ? STATE_TRANSITION_GOTO : STATE_TRANSITION_INIT;

		if( !nextState->canEnterState( transition ) )
		{
			delete nextState;
			return false;
		}

        if( transition.type == STATE_TRANSITION_GOTO )
        {
            BaseStateClass *currentState = m_stackStates.top();

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

template<class BaseStateClass>
template<class StateType, typename ...Args>
bool Statemachine<BaseStateClass>::pushState( Args&& ...args ) 
{
    static_assert( std::is_base_of<BaseStateClass, StateType>::value, 
                   "This StateType can't be used with this statemachine! It inherits from a different base state class than that of statemachine's!" );
    static_assert( std::is_base_of< Statemachine<BaseStateClass>, class StateType::StatemachineType>::value, 
                   "This StateType can't be used with this statemachine! It expects a different statemachine class!" );


    if( m_isCurrentlyLeavingAState )
    {
        return false;
    }

    StateTransition transition;
	transition.prevState = getCurrentStateType();
	transition.nextState = typeid( StateType );
	
    if( transition.prevState != transition.nextState )
    {
        StateType *nextState = new StateType( std::forward<Args>(args)... );
        nextState->setParent( static_cast<typename StateType::StatemachinePtrType>( this ) );

		transition.type = ( m_stackStates.size() >= 1 ) ? STATE_TRANSITION_PUSH : STATE_TRANSITION_INIT;

		if( !nextState->canEnterState( transition ) )
		{
			delete nextState;
			return false;
		}

        if( transition.type == STATE_TRANSITION_PUSH )
        {
            BaseStateClass *currentState = m_stackStates.top();

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

template<class BaseStateClass>
bool Statemachine<BaseStateClass>::popState() 
{
    if( m_isCurrentlyLeavingAState )
    {
        return false;
    }

    // we want to always retain the init state on the stack
    if( m_stackStates.size() > 1 )
    {
        BaseStateClass *currentState = m_stackStates.top();
        std::type_index currentStateType = typeid( *currentState );

        m_stackStates.pop();

        BaseStateClass *nextState = m_stackStates.top();
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

        return true;
    }

	return false;
}

} // namespace chestnut::fsm

#include <type_traits>
#include <cstdio>

namespace chestnut::fsm
{  

inline StatemachineBase::StatemachineBase() 
{
    m_isCurrentlyLeavingAState = false;
}

inline StatemachineBase::~StatemachineBase() noexcept
{
    m_isCurrentlyLeavingAState = true;

    StateTransition transition;
    transition.type = STATE_TRANSITION_DESTROY;
    transition.nextState = NULL_STATE;

    while( !m_stackStates.empty() )
    {
        BaseStateType *state = m_stackStates.top();
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

inline StatemachineBase::BaseStateType* StatemachineBase::getCurrentState() const noexcept
{
    if( !m_stackStates.empty() )
    {
        return m_stackStates.top();
    }

    return nullptr;
}

inline std::type_index StatemachineBase::getCurrentStateType() const noexcept
{
    if( !m_stackStates.empty() )
    {
        return std::type_index( typeid( *m_stackStates.top() ) );
    }

    return NULL_STATE;
}

template<class StateType>
inline bool StatemachineBase::isCurrentlyInState() const
{
    if( !m_stackStates.empty() )
    {
        return std::type_index( typeid( *m_stackStates.top() ) ) == std::type_index( typeid( StateType ) );
    }

    return false;
}

inline int StatemachineBase::getStateStackSize() const noexcept
{
    return (int)m_stackStates.size();
}

template<class StateType, typename ...Args>
inline bool StatemachineBase::initState( Args&& ...args ) 
{
    static_assert( std::is_base_of<StateBase, StateType>::value, "StateType is not a valid state class! It does not inherit from chestnut::fsm::StateBase!" );


    if( !m_stackStates.empty() )
    {
		return false;
    }

	StateTransition transition;
	transition.type = STATE_TRANSITION_INIT;
	transition.prevState = NULL_STATE;
	transition.nextState = typeid( StateType );

    // this can throw BadParentAccessException, but the memory for pointer won't leak
	BaseStateType *initialState = new StateType( std::forward<Args>(args)... );
    if( !initialState->setParent( this ) )
    {
        // polymorphism is needed to check the condition, so this akward immediate deletion after failure is necessary unfortunatelly
        delete initialState;
        return false;
    }

	if( initialState->canEnterState( transition ) )
	{
		m_stackStates.push( initialState );

		try
		{
			initialState->onEnterState( transition );
		}
		catch(const std::exception& e)
		{
            // the init state stays on the stack, but its condition is undefined
			throw OnEnterStateException( transition, e.what() );   
		}

		return true;
	}
	else
	{
		delete initialState;
		return false;
	}
}

template<class StateType, typename ...Args>
inline bool StatemachineBase::gotoState( Args&& ...args ) 
{
    static_assert( std::is_base_of<StateBase, StateType>::value, "StateType is not a valid state class! It does not inherit from chestnut::fsm::StateBase!" );


    if( m_isCurrentlyLeavingAState )
    {
        return false;
    }

	StateTransition transition;
	transition.prevState = getCurrentStateType();
	transition.nextState = typeid( StateType );
	
    if( transition.prevState != transition.nextState )
    {
        // this can throw BadParentAccessException, but the memory for pointer won't leak
        BaseStateType *nextState = new StateType( std::forward<Args>(args)... );
        if( !nextState->setParent( this ) )
        {
            delete nextState;
            return false;
        }

		transition.type = ( !m_stackStates.empty() ) ? STATE_TRANSITION_GOTO : STATE_TRANSITION_INIT;

		if( !nextState->canEnterState( transition ) )
		{
			delete nextState;
			return false;
		}

        if( transition.type == STATE_TRANSITION_GOTO )
        {
            BaseStateType *currentState = m_stackStates.top();

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
                throw OnLeaveStateException( transition, e.what() );    
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
			throw OnEnterStateException( transition, e.what() );
		}

		return true;
    }

    return false;
}

template<class StateType, typename ...Args>
inline bool StatemachineBase::pushState( Args&& ...args ) 
{
    static_assert( std::is_base_of<StateBase, StateType>::value, "StateType is not a valid state class! It does not inherit from chestnut::fsm::StateBase!" );


    if( m_isCurrentlyLeavingAState )
    {
        return false;
    }

    StateTransition transition;
	transition.prevState = getCurrentStateType();
	transition.nextState = typeid( StateType );
	
    if( transition.prevState != transition.nextState )
    {
        // this can throw BadParentAccessException, but the memory for pointer won't leak
        BaseStateType *nextState = new StateType( std::forward<Args>(args)... );
        if( !nextState->setParent( this ) )
        {
            delete nextState;
            return false;
        }

		transition.type = ( !m_stackStates.empty() ) ? STATE_TRANSITION_PUSH : STATE_TRANSITION_INIT;

		if( !nextState->canEnterState( transition ) )
		{
			delete nextState;
			return false;
		}

        if( transition.type == STATE_TRANSITION_PUSH )
        {
            BaseStateType *currentState = m_stackStates.top();

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
                throw OnLeaveStateException( transition, e.what() );    
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
			throw OnEnterStateException( transition, e.what() );
		}

		return true;
    }

    return false;
}

inline bool StatemachineBase::popState() 
{
    if( m_isCurrentlyLeavingAState )
    {
        return false;
    }

    // we want to always retain the init state on the stack
    if( m_stackStates.size() > 1 )
    {
        BaseStateType *currentState = m_stackStates.top();
        std::type_index currentStateType = typeid( *currentState );

        m_stackStates.pop();

        BaseStateType *nextState = m_stackStates.top();
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
            throw OnLeaveStateException( transition, e.what() );
        }
        
        delete currentState;

        m_isCurrentlyLeavingAState = false;


        try
        {
            nextState->onEnterState( transition );
        }
        catch(const std::exception& e)
        {
            throw OnEnterStateException( transition, e.what() );
        }

        return true;
    }

	return false;
}

} // namespace chestnut::fsm

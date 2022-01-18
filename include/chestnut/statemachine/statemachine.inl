#include <type_traits>
#include <cstdio>

namespace chestnut::fsm
{  

template<typename StateInterface>
IStatemachine<StateInterface>::IStatemachine() 
{
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
void IStatemachine<StateInterface>::init( Args&& ...args ) 
{
    static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

    if( m_stackStates.empty() )
    {
        StateInterface *initState = new StateType( dynamic_cast<typename StateInterface::ParentStatemachinePtrType>( this ), std::forward<Args>(args)... );
        m_stackStates.push( initState );

        StateTransition transition;
        transition.type = STATE_TRANSITION_INIT;
        transition.prevState = NULL_STATE;
        transition.nextState = typeid( *initState );

        try
        {
            initState->onEnterState( transition );
        }
        catch(const std::exception& e)
        {
            throw OnEnterStateException( e.what() );   
        }
    }
}

template<typename StateInterface>
template<class StateType, typename ...Args>
void IStatemachine<StateInterface>::gotoState( Args&& ...args ) 
{
    static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

    if( m_isCurrentlyLeavingAState )
    {
        return;
    }

    std::type_index currentStateType = getCurrentStateType();
    std::type_index nextStateType = typeid(StateType);

    if( nextStateType != currentStateType )
    {
        StateInterface *nextState = new StateType( dynamic_cast<typename StateInterface::ParentStatemachinePtrType>( this ), std::forward<Args>(args)... );

        StateTransition transition;
        transition.prevState = currentStateType;
        transition.nextState = nextStateType;

        if( m_stackStates.size() >= 1 )
        {
            StateInterface *currentState = m_stackStates.top();
            transition.type = STATE_TRANSITION_GOTO;

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

            if( m_stackStates.size() > 1)
            {
                m_stackStates.pop();
                delete currentState;
            }

            m_isCurrentlyLeavingAState = false;


            m_stackStates.push( nextState );

            try
            {
                nextState->onEnterState( transition );
            }
            catch(const std::exception& e)
            {
                throw OnEnterStateException( e.what() );
            }
        }
        else
        {
            // like this we can use this method to initialize the statemachine
            transition.type = STATE_TRANSITION_INIT;

            m_stackStates.push( nextState );

            try
            {
                nextState->onEnterState( transition );
            }
            catch(const std::exception& e)
            {
                throw OnEnterStateException( e.what() );
            }                
        }
    }
}

template<typename StateInterface>
template<class StateType, typename ...Args>
void IStatemachine<StateInterface>::pushState( Args&& ...args ) 
{
    static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

    if( m_isCurrentlyLeavingAState )
    {
        return;
    }

    std::type_index currentStateType = getCurrentStateType();
    std::type_index nextStateType = typeid(StateType);

    if( nextStateType != currentStateType )
    {
        StateInterface *nextState = new StateType( dynamic_cast<typename StateInterface::ParentStatemachinePtrType>( this ), std::forward<Args>(args)... );

        StateTransition transition;
        transition.prevState = currentStateType;
        transition.nextState = nextStateType;

        if( m_stackStates.size() >= 1 )
        {
            StateInterface *currentState = m_stackStates.top();
            transition.type = STATE_TRANSITION_PUSH;

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


            m_stackStates.push( nextState );

            try
            {
                nextState->onEnterState( transition );
            }
            catch(const std::exception& e)
            {   
                throw OnEnterStateException( e.what() );
            }
        }
        else
        {
            // like this we can use this method to initialize the statemachine
            transition.type = STATE_TRANSITION_INIT;

            m_stackStates.push( nextState );

            try
            {
                nextState->onEnterState( transition );
            }
            catch(const std::exception& e)
            {
                throw OnEnterStateException( e.what() );
            }    
        }
    }
}

template<class StateInterface>
void IStatemachine<StateInterface>::popState() 
{
    if( m_isCurrentlyLeavingAState )
    {
        return;
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

        m_isCurrentlyLeavingAState = true;

        try
        {
            currentState->onLeaveState( transition );
        }
        catch(const std::exception& e)
        {
            m_isCurrentlyLeavingAState = false;
            // we're gonna push this state back so that SM goes back to as it was before except now its condition is undefined
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
}

} // namespace chestnut::fsm

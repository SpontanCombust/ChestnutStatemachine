#include <type_traits>
#include <cstdio>

namespace chestnut::fsm
{  
    template<class StateInterface>
    IStatemachine<StateInterface>::~IStatemachine() noexcept
    {
        while( !m_stackStates.empty() )
        {
            StateInterface *state = m_stackStates.top();
            m_stackStates.pop();

            try
            {
                state->onExit( NULL_STATE );
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
    int IStatemachine<StateInterface>::getStateStackSize() const noexcept
    {
        return (int)m_stackStates.size();
    }

    template<typename StateInterface>
    template<class StateType>
    void IStatemachine<StateInterface>::init() 
    {
        static_assert( std::is_base_of<StateInterface, StateType>::value, "StateType must have StateInterface as its parent class!" );

        if( m_stackStates.empty() )
        {
            StateInterface *initState = new StateType( dynamic_cast<typename StateInterface::ParentStatemachinePtrType>( this ) );    

            m_stackStates.push( initState );

            try
            {
                initState->onEnter( NULL_STATE );
            }
            catch(const std::exception& e)
            {
                throw OnEnterException( e.what() );   
            }
        }
    }

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

				try
				{
					currentState->onExit( nextStateType );
				}
				catch(const std::exception& e)
				{
					delete nextState;
					throw OnExitException( e.what() );    
				}

				if( m_stackStates.size() > 1)
				{
					m_stackStates.pop();
					delete currentState;
				}


                m_stackStates.push( nextState );

                try
                {
                    nextState->onEnter( currentStateType );
                }
                catch(const std::exception& e)
                {
                    throw OnEnterException( e.what() );
                }
            }
            else
            {
				// like this we can use this method to initialize the statemachine

                m_stackStates.push( nextState );

                try
                {
                    nextState->onEnter( NULL_STATE );
                }
                catch(const std::exception& e)
                {
                    throw OnEnterException( e.what() );
                }                
            }
        }
    }

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

                try
                {
                    currentState->onExit( nextStateType );
                }
                catch(const std::exception& e)
                {
                    delete nextState;
                    throw OnExitException( e.what() );    
                }


                m_stackStates.push( nextState );

                try
                {
                    nextState->onEnter( currentStateType );
                }
                catch(const std::exception& e)
                {   
                    throw OnEnterException( e.what() );
                }
            }
            else
            {
				// like this we can use this method to initialize the statemachine

                m_stackStates.push( nextState );

                try
                {
                    nextState->onEnter( NULL_STATE );
                }
                catch(const std::exception& e)
                {
                    throw OnEnterException( e.what() );
                }    
            }
        }
    }

    template<class StateInterface>
    void IStatemachine<StateInterface>::popState() 
    {
        // we want to always retain the init state on the stack
        if( m_stackStates.size() > 1 )
        {
            StateInterface *currentState = m_stackStates.top();
			std::type_index currentStateType = typeid( *currentState );

            m_stackStates.pop();

            StateInterface *nextState = m_stackStates.top();
			std::type_index nextStateType = typeid( *nextState );

			try
			{
				currentState->onExit( nextStateType );
			}
			catch(const std::exception& e)
			{
				// we're gonna push this state back so that SM goes back to as it was before except now its condition is undefined
				m_stackStates.push( currentState );
				throw OnExitException( e.what() );
			}
			
            delete currentState;


            try
			{
            	nextState->onEnter( currentStateType );
			}
			catch(const std::exception& e)
			{
				throw OnEnterException( e.what() );
			}
        }
    }

} // namespace chestnut::fsm

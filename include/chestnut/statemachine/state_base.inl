namespace chestnut::fsm
{

inline bool StateBase::canEnterState( StateTransition transition ) const noexcept
{
    return true;
}

inline bool StateBase::canLeaveState( StateTransition transition ) const noexcept
{
    return true;
}

inline void StateBase::onEnterState( StateTransition transition ) 
{
    /*NOP*/
}

inline void StateBase::onLeaveState( StateTransition transition ) 
{
    /*NOP*/
}

} // namespace chestnut::fsm

namespace chestnut::fsm
{

inline bool IStateBase::canEnterState( StateTransition transition ) const noexcept
{
    return true;
}

inline bool IStateBase::canLeaveState( StateTransition transition ) const noexcept
{
    return true;
}

inline void IStateBase::onEnterState( StateTransition transition ) 
{
    /*NOP*/
}

inline void IStateBase::onLeaveState( StateTransition transition ) 
{
    /*NOP*/
}

} // namespace chestnut::fsm

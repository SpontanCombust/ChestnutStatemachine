namespace chestnut::fsm
{

inline bool IState_Base::canEnterState( StateTransition transition ) const noexcept
{
    return true;
}

inline bool IState_Base::canLeaveState( StateTransition transition ) const noexcept
{
    return true;
}

inline void IState_Base::onEnterState( StateTransition transition ) 
{
    /*NOP*/
}

inline void IState_Base::onLeaveState( StateTransition transition ) 
{
    /*NOP*/
}

} // namespace chestnut::fsm

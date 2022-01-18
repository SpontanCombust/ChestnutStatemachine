namespace chestnut::fsm
{
        
template<class ParentStatemachine>
IState<ParentStatemachine>::IState( ParentStatemachine *parent_ ) 
{
    parent = parent_;
}

template<class ParentStatemachine>
bool IState<ParentStatemachine>::canEnterState( StateTransition transition ) const noexcept
{
    return true;
}

template<class ParentStatemachine>
bool IState<ParentStatemachine>::canLeaveState( StateTransition transition ) const noexcept
{
    return true;
}

} // namespace chestnut::fsm

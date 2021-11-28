namespace chestnut::fsm
{    
    template<class ParentStatemachine>
    IState<ParentStatemachine>::IState( ParentStatemachine *parent_ ) 
    {
        parent = parent_;
    }

} // namespace chestnut::fsm

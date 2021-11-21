namespace chestnut::statemachine
{    
    template<typename StateNameType, class ParentStatemachine>
    IState<StateNameType, ParentStatemachine>::IState( ParentStatemachine *parent_ ) 
    {
        parent = parent_;
    }

} // namespace chestnut::statemachine

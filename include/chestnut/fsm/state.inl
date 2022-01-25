namespace chestnut::fsm
{

template<class ParentStatemachineClass, class BaseStateClass>
typename State<ParentStatemachineClass, BaseStateClass>::StatemachineType& State<ParentStatemachineClass, BaseStateClass>::getParent() 
{
    // setParent ensures to the statemachine the parent pointer is valid 
    // so this will either be null or valid StatemachineType
    return *static_cast<StatemachineType*>( this->parent );
}

template<class ParentStatemachineClass, class BaseStateClass>
const typename State<ParentStatemachineClass, BaseStateClass>::StatemachineType& State<ParentStatemachineClass, BaseStateClass>::getParent() const 
{
    return *static_cast<StatemachineType*>( this->parent );
}

template<class ParentStatemachineClass, class BaseStateClass>
bool State<ParentStatemachineClass, BaseStateClass>::setParent( StatemachineBase *parent_ ) noexcept
{
    if( dynamic_cast<StatemachineType*>( parent_ ) )
    {
        this->parent = parent_;
        return true;
    }

    return false;
}


template<class ParentStatemachineClass>
typename State<ParentStatemachineClass, void>::StatemachineType& State<ParentStatemachineClass, void>::getParent() 
{
    // setParent ensures to the statemachine the parent pointer is valid 
    // so this will either be null or valid StatemachineType
    return *static_cast<StatemachineType*>( this->parent );
}

template<class ParentStatemachineClass>
const typename State<ParentStatemachineClass, void>::StatemachineType& State<ParentStatemachineClass, void>::getParent() const 
{
    return *static_cast<StatemachineType*>( this->parent );
}

template<class ParentStatemachineClass>
bool State<ParentStatemachineClass, void>::setParent( StatemachineBase *parent_ ) noexcept
{
    if( dynamic_cast<StatemachineType*>( parent_ ) )
    {
        this->parent = parent_;
        return true;
    }

    return false;
}

} // namespace chestnut::fsm

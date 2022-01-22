namespace chestnut::fsm
{
        
template<class ParentStatemachineClass, class BaseStateClass>
State<ParentStatemachineClass, BaseStateClass>::State() noexcept
{
    static_assert( std::is_base_of< fsm::StateBase, BaseStateClass >::value, "BaseStateClass is not a valid class for this statemachine! It does not iherit from fsm::StateBase!" );

    // even if the parent should always be set right after constructor, it's safer to make it null just to be sure
    // If someone tries to use the parent in the constructor at least it will be null and not have some random address
    this->parent = nullptr;
}

template<class ParentStatemachineClass, class BaseStateClass>
void State<ParentStatemachineClass, BaseStateClass>::setParent( StatemachinePtrType parent ) noexcept
{
    this->parent = parent;
}

} // namespace chestnut::fsm

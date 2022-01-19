namespace chestnut::fsm
{
        
template<class ParentStatemachine>
IState<ParentStatemachine>::IState() noexcept
{
    // even if the parent should always be set right after constructor, it's safer to make it null just to be sure
    this->parent = nullptr;
}

template<class ParentStatemachine>
void IState<ParentStatemachine>::setParent( ParentStatemachinePtrType parent ) noexcept
{
    this->parent = parent;
}

} // namespace chestnut::fsm

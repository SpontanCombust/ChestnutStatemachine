namespace chestnut::fsm
{
        
template<class ParentStatemachineClass>
State<ParentStatemachineClass>::State() noexcept
{
    // even if the parent should always be set right after constructor, it's safer to make it null just to be sure
    // If someone tries to use the parent in the constructor at least it will be null and not have some random address
    this->parent = nullptr;
}

template<class ParentStatemachineClass>
void State<ParentStatemachineClass>::setParent( StatemachinePtrType parent ) noexcept
{
    this->parent = parent;
}

} // namespace chestnut::fsm

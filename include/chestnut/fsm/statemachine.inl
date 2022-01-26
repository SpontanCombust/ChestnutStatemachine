#include <type_traits>

namespace chestnut::fsm
{

template<class StateExtension, class BaseStatemachineClass>
Statemachine<StateExtension, BaseStatemachineClass>::Statemachine() 
{
    static_assert( std::is_base_of<StatemachineBase, BaseStatemachineClass>::value, 
        "BaseStatemachineClass has to be a child of StatemachineBase!" );
}

template<class StateExtension, class BaseStatemachineClass>
typename Statemachine<StateExtension, BaseStatemachineClass>::BaseStateType* Statemachine<StateExtension, BaseStatemachineClass>::getCurrentState() const noexcept
{
    return dynamic_cast< BaseStateType* >( BaseStatemachineClass::getCurrentState() );   
}

template<class BaseStatemachineClass>
Statemachine<void,BaseStatemachineClass>::Statemachine() 
{
    static_assert( std::is_base_of<StatemachineBase, BaseStatemachineClass>::value, 
        "BaseStatemachineClass has to be a child of StatemachineBase!" );
}

} // namespace chestnut::fsm

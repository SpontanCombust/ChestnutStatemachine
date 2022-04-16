#include <type_traits>

namespace chestnut::fsm
{

template<class StateExtension, class BaseStatemachineClass>
Statemachine<StateExtension, BaseStatemachineClass>::Statemachine() 
{
    static_assert( std::is_base_of<StatemachineBase, BaseStatemachineClass>::value, 
        "BaseStatemachineClass has to be a child of StatemachineBase!" );
    
    using BaseStateExtensionType = typename BaseStatemachineClass::StateExtensionType;
    if constexpr( !std::is_same<void, BaseStateExtensionType>::value )
    {
        static_assert( std::is_base_of<BaseStateExtensionType, StateExtension>::value, 
            "StateExtension should be a subclass of the extension type from the BaseStatemachineClass!" );
    }
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

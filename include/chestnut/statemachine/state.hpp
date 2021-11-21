#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

namespace chestnut::statemachine
{
    /**
     * @brief Base generic abstract class used for creating state interface
     * 
     * @tparam StateNameType a type of states' name
     * @tparam ParentStatemachine type of the statemachine that will house the state
     * 
     * @details
     * ParentStatemachine type should be forward declared before the state interface.
     * 
     * StateNameType can be any type you'd want to identify your states by, like an enum, string or a regular int.
     * The type must however be able to be used for std::unordered_map as a key,
     * so if you have a custom type you would want to use a name, you need to implement a std::hash template specialization for it
     */
    template< typename StateNameType, class ParentStatemachine >
    class IState
    {
    protected:
        ParentStatemachine *parent; /*Pointer to the statemachine that houses the state*/

    public:
        typedef ParentStatemachine* ParentStatemachinePtrType;
        StateNameType name; /*Custom name for any state that may inherit from the interface*/

    public:
        IState( ParentStatemachine *parent_ );
        virtual ~IState() = default;
        
        /**
         * @brief A method called whenever statemachine enters this state
         * 
         * @param prevState name of the state statemachine swithed from
         */
        virtual void onEnter( StateNameType prevState ) = 0;
        /**
         * @brief A method called whenever statemachine exits this state
         * 
         * @param nextState name of the state statemachine is swithing to
         */
        virtual void onExit( StateNameType nextState ) = 0;
    };
    
} // namespace chestnut::statemachine


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
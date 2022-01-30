/**
 * @file state.hpp
 * @author Przemysław Cedro (SpontanCombust)
 * @brief Header file with the template State class
 * @version 3.0.0
 * @date 2022-01-30
 * 
 * @copyright MIT License (c) 2021-2022
 * 
 */

#ifndef __CHESTNUT_STATEMACHINE_STATE_H__
#define __CHESTNUT_STATEMACHINE_STATE_H__

#include "state_base.hpp"

namespace chestnut::fsm
{

/**
 * @brief Template state class. Inherit from this class to create a proper state class
 * 
 * @details
 * If you want not only to create a state of some statemachine, but also extend some other state class that has different statemachine class as its parent
 * (that state should be a state of a base class to the ParentStatemachineClass) make sure to enter its type as the 2nd template parameter and not
 * do an inheritance the regular way. The inheritance is handled by the template to avoid unwanted behaviour.
 * 
 * This class uses a few C++ idioms that help it cover up the complex nature of properly setting up a state class.
 * These idioms mainly being "Curiously Recurring Template Pattern" (class automatically derives from types provided from template parameters)
 * and "Co-variant Return Types" (return types of getParent are overriden to those of the target statemachine type).
 * 
 * @tparam ParentStatemachineClass type of the statemachine
 * @tparam BaseStateClass optional base state class to derive from
 */
template< class ParentStatemachineClass, class BaseStateClass = void >
class State : virtual public ParentStatemachineClass::BaseStateType, virtual public BaseStateClass
{
public:
    /**
     * @brief Typedef of the parent statemachine type; overrides the typedef from the base state class
     */
    typedef ParentStatemachineClass StatemachineType;
    /**
     * @brief Typedef of the base class to this state class; overrides the typedef from the base state class
     */
    typedef class ParentStatemachineClass::BaseStateType BaseStateType;

public:
    /**
     * @brief Get the parent statemachine reference
     *  
     * @return parent statemachine reference
     * 
     * @throws BadParentAccessException if used before the parent pointer has been set (this will happen if used in a constructor)
     */
    virtual StatemachineType& getParent() override;
    /**
     * @brief Get the parent statemachine reference
     *  
     * @return parent statemachine reference
     * 
     * @throws BadParentAccessException if used before the parent pointer has been set (this will happen if used in a constructor)
     */
    virtual const StatemachineType& getParent() const override;

private:
    /**
     * @brief Set the parent pointer. This method is called internally from the statemachine
     * 
     * @param parent_ parent statemachine pointer
     * @return Returns whether this state type can be bound to a given statemachine type
     */
    virtual bool setParent( StatemachineBase *parent_ ) noexcept override;
};


// This is a specialization for when there is no additional state class to inherit from.
// So in this scenario this inherits only from statemachine's base state type.
template< class ParentStatemachineClass >
class State<ParentStatemachineClass, void> : virtual public ParentStatemachineClass::BaseStateType
{
public:
    typedef ParentStatemachineClass StatemachineType;
    typedef class ParentStatemachineClass::BaseStateType BaseStateType;

public:
    virtual StatemachineType& getParent() override;
    virtual const StatemachineType& getParent() const override;

private:
    virtual bool setParent( StatemachineBase *parent_ ) noexcept override;
};

} // namespace chestnut::fsm


#include "state.inl"


#endif // __CHESTNUT_STATEMACHINE_STATE_H__
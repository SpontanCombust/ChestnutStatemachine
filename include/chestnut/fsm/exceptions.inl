namespace chestnut::fsm
{

inline StatemachineException::StatemachineException( const char *msg ) throw()
{
    this->message = msg;
}

inline const char* StatemachineException::what() const throw() 
{
    return this->message.c_str();
}

inline BadParentAccessException::BadParentAccessException( const char *msg ) : StatemachineException( msg ) 
{

}

inline StateChangeException::StateChangeException( StateTransition transition, const char *msg ) throw() : StatemachineException( msg )
{
    this->transition = transition;
}

inline OnEnterStateException::OnEnterStateException( StateTransition transition, const char *msg ) throw() : StateChangeException( transition, msg )
{
    message = "Exception was thrown when entering a state: " + message;
}

inline OnLeaveStateException::OnLeaveStateException( StateTransition transition, const char *msg ) throw() : StateChangeException( transition, msg )
{
    message = "Exception was thrown when leaving a state: " + message;
}

} // namespace chestnut::fsm

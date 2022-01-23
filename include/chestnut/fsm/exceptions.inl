namespace chestnut::fsm
{
    
inline StateChangeException::StateChangeException( const char *msg ) throw()
{
    message = msg;
}

inline const char* StateChangeException::what() const throw()
{
    return message.c_str();
}

inline OnEnterStateException::OnEnterStateException( const char *msg ) throw() : StateChangeException( msg )
{
    message = "Exception was thrown when entering a state: " + message;
}

inline OnLeaveStateException::OnLeaveStateException( const char *msg ) throw() : StateChangeException( msg )
{
    message = "Exception was thrown when leaving a state: " + message;
}

} // namespace chestnut::fsm

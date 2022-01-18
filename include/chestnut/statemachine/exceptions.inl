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

inline OnEnterException::OnEnterException( const char *msg ) throw() : StateChangeException( msg )
{
    message = "Exception was thrown when entering a state: " + message;
}

inline OnExitException::OnExitException( const char *msg ) throw() : StateChangeException( msg )
{
    message = "Exception was thrown when exiting a state: " + message;
}

} // namespace chestnut::fsm

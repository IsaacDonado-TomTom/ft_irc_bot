#include "AutoReply.hpp"

AutoReply::AutoReply()
{
    return ;
}

AutoReply::~AutoReply()
{
    return ;
}

void    AutoReply::setReply(const std::string& sender, const std::string& msg)
{
    this->_sender = sender;
    this->_msg = msg;

    if (msg == "time" || msg == "time.")
        this->_reply = "PRIVMSG " + this->_sender + " :" + "The time is 12:00\r\n";
    else
        this->_reply = "PRIVMSG " + this->_sender + " :" + "Come again?\r\n";
    return ;
}

void    AutoReply::sendReply(int socket_fd)
{
    int send_result = send(socket_fd, this->_reply.c_str(), this->_reply.size() + 1, 0);
    if (send_result == -1)
        std::cerr << "Unable to send data to server.\n";
    return ;
}

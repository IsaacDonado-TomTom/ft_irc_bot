#include "AutoReply.hpp"

AutoReply::AutoReply()
{
    return ;
}

AutoReply::~AutoReply()
{
    return ;
}

void    AutoReply::trimMsg(void)
{
    size_t i = 0;
    while (this->_msg[i] == ' ')
    {
        i++;
    }

    this->_msg = std::string(this->_msg.c_str(), i, std::string::npos);

    i = 0;
    while (this->_msg[i] != '\0')
    {
        i++;
    }
    if (i > 0)
    {
        i--;
        while (this->_msg[i] == ' ')
        {
            i--;
        }
        this->_msg = std::string(this->_msg.c_str(), 0, i+1);
    }
    return ;
}

void    AutoReply::setReply(const std::string& sender, const std::string& msg)
{
    this->_sender = sender;
    this->_msg = msg;
    this->trimMsg();

    if (this->_msg == "time" || this->_msg == "time.")
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

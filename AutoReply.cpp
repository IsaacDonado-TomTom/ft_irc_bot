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

std::string AutoReply::getTime(void)
{
    time_t t;

    time(&t);
    std::string result = std::string(ctime(&t), 11, 5);
    return (result);
}

std::string AutoReply::getDate(void)
{
    time_t t;

    time(&t);
    std::string result = std::string(ctime(&t), 4, 7) + std::string(ctime(&t), 20, 4);
    return (result);
}

std::string AutoReply::getDay(void)
{
    time_t t;

    time(&t);
    std::string result = std::string(ctime(&t), 0, 3);
    if (result == "Fri")
        result = "Friday";
    else if (result == "Sat")
        result = "Saturday";
    else if (result == "Sun")
        result = "Sunday";
    else if (result == "Mon")
        result = "Monday";
    else if (result == "Tue")
        result = "Tuesday";
    else if (result == "Wed")
        result = "Wednesday";
    else
        result = "Thursday";
    return (result);
}

void    AutoReply::setReply(const std::string& sender, const std::string& msg)
{
    this->_sender = sender;
    this->_msg = msg;
    this->trimMsg();

    if (strcasecmp(this->_msg.c_str(), "time") == 0 || strcasecmp(this->_msg.c_str(), "time.") == 0)
    {
        this->_reply = "PRIVMSG " + this->_sender + " :" + "The time is " + this->getTime() + "\r\n";
    }
    else if (strcasecmp(this->_msg.c_str(), "date") == 0 || strcasecmp(this->_msg.c_str(), "date.") == 0)
    {
        this->_reply = "PRIVMSG " + this->_sender + " :" + this->getDate() + "\r\n";
    }
    else if (strcasecmp(this->_msg.c_str(), "day") == 0 || strcasecmp(this->_msg.c_str(), "day.") == 0)
    {
        this->_reply = "PRIVMSG " + this->_sender + " :" + this->getDay() + "\r\n";
    }
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

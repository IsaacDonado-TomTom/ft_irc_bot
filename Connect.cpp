#include "Connect.hpp"

Connect::Connect(ArgParser* args)
{
    this->args = args;
    return ;
}

Connect::~Connect()
{
    // Done
}

void    Connect::setSocket(void)
{
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd == -1)
    {
        std::cerr << "Bonus bot failed creating socket.\n";
        exit (1);
    }
    return ;
}

void    Connect::setHint(void)
{
    this->hint.sin_family = AF_INET;
    this->hint.sin_port = htons(this->args->getPort());
    this->hint.sin_addr.s_addr = inet_addr(this->args->getAddress().c_str());
}

int     Connect::reconnect(void)
{
    std::cout << "Unable to connect, Attempting to reconnect after 5 seconds...\n";
    usleep(5000000);
    close(this->socket_fd);
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd == -1)
    {
        std::cerr << "Bonus bot failed creating socket when reconnecting.\n";
        exit (1);
    }
    if (::connect(this->socket_fd, (sockaddr*)&this->hint, (socklen_t)(sizeof(this->hint))) == -1)
    {
        std::cerr << "Reconnection failed.\n";
        return (-1);
    }
    this->authenticate();
    return (0);
}

int    Connect::connect(void)
{
    if (::connect(this->socket_fd, (sockaddr*)&this->hint, (socklen_t)(sizeof(this->hint))) == -1)
    {
        std::cerr << "Connection failed.\n";
        return(-1);
    }
    return (0);
}

void    Connect::authenticate(void)
{
    std::string input;

    input = "CAPLS \r\nPASS " + args->getPassword() + "\r\nNICK bonusbot\r\nUSER bonusbot bonusbot " + args->getAddress() + " :Bonus Bot\r\n";
    if (send(socket_fd, input.c_str(), input.size() + 1, 0) == -1)
    {
        std::cerr << "Authentification failed.\n";
        close(this->socket_fd);
        exit(1);
    }
    return ;
}

int     Connect::getSocket(void)
{
    return (this->socket_fd);
}

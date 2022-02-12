#ifndef CONNECT_HPP
# define CONNECT_HPP
# include "ArgParser.hpp"
# include <sys/types.h> // for connect()
# include <sys/socket.h> // for structs
# include <unistd.h> // for close() and usleep()

class Connect
{
    public:
        Connect(ArgParser* args);
        ~Connect();
        void    setHint(void);
        void    setSocket(void);
        int     getSocket(void);
        int     connect(void);
        int     reconnect();
        void    authenticate(void);
    private:
        ArgParser*  args;
        int         socket_fd;
        sockaddr_in hint;
};

#endif

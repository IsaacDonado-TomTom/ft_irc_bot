#ifndef AUTOREPLY_HPP
# define AUTOREPLY_HPP

# include <iostream>
# include <sys/socket.h>
# include <time.h>
# include <cstring>

class   AutoReply
{
    public:
        AutoReply();
        ~AutoReply();
        void    setReply(const std::string& sender, const std::string& msg);
        void    sendReply(int fd);
    private:
        void        trimMsg(void);
        bool        isJoinRequest(void);
        void        joinRequest(void);
        bool        sendPartial(int socket_fd);
        std::string getTime(void);
        std::string getDate(void);
        std::string getDay(void);
        std::string _sender;
        std::string _msg;
        std::string _reply;
};

#endif

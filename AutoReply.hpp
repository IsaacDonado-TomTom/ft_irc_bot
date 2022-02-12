#ifndef AUTOREPLY_HPP
# define AUTOREPLY_HPP

# include <iostream>
# include <sys/socket.h>

class   AutoReply
{
    public:
        AutoReply();
        ~AutoReply();
        void    setReply(const std::string& sender, const std::string& msg);
        void    sendReply(int fd);
    private:
        void    trimMsg(void);
        std::string _sender;
        std::string _msg;
        std::string _reply;
};

#endif

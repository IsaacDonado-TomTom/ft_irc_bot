#include <iostream>
#include "ArgParser.hpp"
#include "AutoReply.hpp"
#include <sys/socket.h> // for socket()
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> // for usleep()
#include <netdb.h> // for gethostbyname

int    reconnect(int socket_fd, sockaddr_in* hint, socklen_t s_len)
{
    std::cout << "Unable to connect, Attempting to reconnect after 5 seconds...\n";
    usleep(5000000);
    int result = connect(socket_fd, (sockaddr*)hint, s_len);
    return (result);
}

bool     authentification(int socket_fd, ArgParser*  args)
{
    std::string input;
    int send_result;

    input = "CAPLS \r\nPASS " + args->getPassword() + "\r\nNICK bonusbot\r\nUSER bonusbot bonusbot " + args->getAddress() + " :Bonus Bot\r\n";
    send_result = send(socket_fd, input.c_str(), input.size() + 1, 0);

    if (send_result == -1)
        return false;

    return true;
}

int main(int argc, char** argv)
{
    ArgParser   args(argc, argv);
    // args[1] args[2] args[3] for arguements 1-3

    //Create socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        std::cout << "Bonus bot failed creating socket.\n";
        return (1);
    }

    // Create hint struct for server we're connecting to.
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(args.getPort());
    hint.sin_addr.s_addr = inet_addr(args.getAddress().c_str());

    // Connect to server
    int connect_result = connect(socket_fd, (sockaddr*)&hint, (socklen_t)(sizeof(hint)));
    while (connect_result == -1)
        connect_result = reconnect(socket_fd, &hint, (socklen_t)(sizeof(hint)));
    

    char        buff[4096];
    int         recv_result;
    std::string str_received;
    std::string sender_nick;
    std::string sender_msg;
    int         pm_pos;
    AutoReply   auto_reply;

    // Authenticate
    if (authentification(socket_fd, &args) == false)
    {
        std::cout << "Authentification failed.\n";
        close(socket_fd);
        return (1);
    }

    do
    {

        // Receive
        memset(buff, 0, 4096);
        recv_result = recv(socket_fd, (void*)buff, 4096, 0);
        if (recv_result == -1)
        {
            std::cout << "Failed to receive data from server.\n";
            continue ;
        }
        else if (recv_result == 0)
        {
            connect_result = -1;
            while (connect_result == -1)
                connect_result = reconnect(socket_fd, &hint, (socklen_t)(sizeof(hint)));
        }
        else
        {// Received from server... Define behaviour
            str_received = std::string(buff, recv_result);
            std::cout << "SERVER> " << str_received << "\r\n";

            pm_pos = str_received.find("PRIVMSG");
            if (str_received.find("PRIVMSG") != std::string::npos)
            {// PRIVMSG FOUND IN MSG
                std::cout << "Received a private message from '";
                sender_msg = std::string(str_received.c_str(), pm_pos + 8, std::string::npos);
                sender_nick = std::string(str_received.c_str(), 1, std::string::npos);
                for(unsigned long i=0;i<sender_nick.length();i++)
                {
                    if (sender_nick[i] == '!')
                    {
                        sender_nick = std::string(sender_nick.c_str(), 0, i);
                        break ;
                    }
                }
                for(unsigned long i=0;i<sender_msg.length();i++)
                {
                    if (sender_msg[i] == ' ')
                    {
                        sender_msg = std::string(sender_msg.c_str(), i+2, sender_msg.find("\r")-1);
                        sender_msg = std::string(sender_msg.c_str(), 0, i);
                        break ;
                    }
                }
                for(unsigned long i=0;i<sender_msg.length();i++)
                {
                    if (sender_msg[i] == '\r' || sender_msg[i] == '\n')
                    {
                        sender_msg = std::string(sender_msg.c_str(), 0, i);
                        break ;
                    }
                }

                std::cout << sender_nick << "'" <<std::endl;
                std::cout << "Msg: '" << sender_msg << "'" << std::endl;

                auto_reply.setReply(sender_nick, sender_msg);
                auto_reply.sendReply(socket_fd);
            }// ENDIF PRIVMSG FOUND
        }


    }
    while (true);
    close(socket_fd);
    return (0);
}

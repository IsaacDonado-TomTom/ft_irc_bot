#include <iostream>
#include "ArgParser.hpp"
#include "AutoReply.hpp"
#include "Connect.hpp"
#include <sys/socket.h> // for socket()
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> // for usleep()
#include <netdb.h> // for gethostbyname

int main(int argc, char** argv)
{
    ArgParser   args(argc, argv);
    // args[1] args[2] args[3] for arguements 1-3

    
    // Initialize connect class
    Connect connection(&args);

    //Create socket
    connection.setSocket();

    //Set server hint
    connection.setHint();

    // Connect to server
    int connect_result = connection.connect();
    while (connect_result == -1)
        connect_result = connection.reconnect();

    char        buff[4096];
    int         recv_result;
    std::string str_received;
    std::string sender_nick;
    std::string sender_msg;
    int         pm_pos;
    AutoReply   auto_reply;

    // Authenticate
    connection.authenticate();

    do
    {

        // Receive
        memset(buff, 0, 4096);
        recv_result = recv(connection.getSocket(), (void*)buff, 4096, 0);
        if (recv_result == -1)
        {
            std::cout << "Failed to receive data from server.\n";
            continue ;
        }
        else if (recv_result == 0)
        {
            connect_result = -1;
            while (connect_result == -1)
                connect_result = connection.reconnect();
        }
        else
        {// Received from server... Define behaviour
            str_received = std::string(buff, recv_result);
            //std::cout << "SERVER> " << str_received << "\r\n";

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

                //std::cout << sender_nick << "'" <<std::endl;
                //std::cout << "Msg: '" << sender_msg << "'" << std::endl;

                auto_reply.setReply(sender_nick, sender_msg);
                auto_reply.sendReply(connection.getSocket());
            }// ENDIF PRIVMSG FOUND
        }


    }
    while (true);
    close(connection.getSocket());
    return (0);
}

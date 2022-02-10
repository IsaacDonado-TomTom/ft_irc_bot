#ifndef ARGPARSER_HPP
# define ARGPARSER_HPP

# include <iostream>
# include <string>
# include <netdb.h>
# include <arpa/inet.h>

class ArgParser
{
    public:
        ArgParser(int argc, char** args);
        ~ArgParser();
        std::string&    operator[](int num);
        int             getPort(void);
        std::string&    getAddress(void);
        std::string&    getPassword(void);
    private:
        void        printUsage(void);
        bool        addressCheck(void);
        bool        portCheck(void);
        std::string _address;
        std::string _port;
        std::string _password;
        int         _portInt;
};

#endif

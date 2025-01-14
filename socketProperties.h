#ifndef SOCKETPROPERTIES_H
#define SOCKETPROPERTIES_H

#include <iostream>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
class socketProperties{
    public:
        //constructor
        socketProperties();

        //getters
        int getSocketAddressFamily() const {return socketAddressFamily; }

        int getSocketType() const {return socketType; }

        unsigned short getSocketPort() const {return socketPort; }

        //setters
        void setSocketAddressFamily(string addressFamily);

        void setSocketType(string socketType);

        void setSocketPort(unsigned short socketPort);
    private:
        enum socketAddressFamily { IPV4 = AF_INET, IPV6 = AF_INET6, UNKNOWN = -1};
        socketAddressFamily socketAddressFamily;

        enum socketType { TCP = SOCK_STREAM, UDP = SOCK_DGRAM, RAW = SOCK_RAW};
        socketType socketType;

        unsigned short socketPort = DEFAULT_PORT;

        static const unsigned short DEFAULT_PORT = 8080;
        static const unsigned short PRIVILEGED_PORT_THRESHOLD = 1024;

        mutex propertyMutex;
};

#endif
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <algorithm>

#include "socketProperties.h"

using namespace std;

socketProperties::socketProperties() {
    socketAddressFamily = UNKNOWN;
    socketType = RAW;
    socketPort = 8080;
}

void socketProperties::setSocketAddressFamily(string addressFamily) {

    transform(addressFamily.begin(), addressFamily.end(), addressFamily.begin(), ::toupper);

    if(addressFamily == "IPV4" || addressFamily == "AF_INET") {
        socketAddressFamily = IPV4;
    }
    else if(addressFamily == "IPV6" || addressFamily == "AF_INET6") {
        socketAddressFamily = IPV6;
    }
    else {
        socketAddressFamily = UNKNOWN;
        cerr << "Warning: Unknown address family. Defaulting to UNKNOWN.\n";
    }
}

void socketProperties::setSocketType(string socketType) {
    transform(socketType.begin(), socketType.end(), socketType.begin(), ::toupper);

    if(socketType == "TCP" || socketType == "SOCK_STREAM") {
        this -> socketType = TCP;
    }
    else if(socketType == "UDP" || socketType == "SOCK_DGRAM") {
        socketType = UDP;
    }
    else if(socketType == "RAW" || socketType == "SOCK_RAW") {
        socketType = RAW;
    }
    else {
        socketType = RAW;
        cerr << "Warning: Unknown socket type. Defaulting to RAW.\n";
    }
}

void socketProperties::setSocketPort(unsigned short port) {
    lock_guard<mutex> lock(propertyMutex);
    if(port >= 1 && port <= 65534) {
        if(port < PRIVILEGED_PORT_THRESHOLD) {
            cerr << "warning: Port number less than 1024. This may require elevated privileges.\n";
        }
        this -> socketPort = port;
    }
    else {
        this -> socketPort = DEFAULT_PORT;
        cerr << "Warning: Invalid port number. Defaulting to " << DEFAULT_PORT << ".\n";
    }
}
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "socketProperties.h"

using namespace std;

int main() {

    //user input to decide socket settings
    socketProperties socketProp;

    socketProp.setSocketAddressFamily("IPV6");
    socketProp.setSocketType("TCP");
    socketProp.setSocketPort(6969);

    //setup the socket
    // using ipv4: AF_INET
    // using TCP: SOCK_STREAM
    int server_socket = socket( socketProp.getSocketAddressFamily(), socketProp.getSocketType(), 0);

    if(server_socket < 0) {
        cerr << "Error: Could not create socket.\n";
        exit(1);
    }

    //bind the socket to IP and port

    //ipv4 bind
    if(socketProp.getSocketAddressFamily() == AF_INET) {
        sockaddr_in server_address{};
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(socketProp.getSocketPort());
        server_address.sin_addr.s_addr = INADDR_ANY; //TODO: change this to allow for dynamic IP address

        if(bind(server_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) <0) {
            cerr << "Error: Could not bind socket to IP and port.\n"; //TODO: specify if it was IP or port that failed
            exit(1);
        }
    }
    //ipv6 bind
    else if(socketProp.getSocketAddressFamily() == AF_INET6) {
        sockaddr_in6 server_address6{};
        server_address6.sin6_family = AF_INET6;
        server_address6.sin6_port = htons(socketProp.getSocketPort());
        server_address6.sin6_addr = in6addr_any; //TODO: change this to allow for dynamic IP address

        if(bind(server_socket, reinterpret_cast<sockaddr*>(&server_address6), sizeof(server_address6)) <0) {
            cerr << "Error: Could not bind socket to IP and port.\n"; //TODO: specify if it was IP or port that failed
            exit(1);
        }

    }
    //if address family was never set
    else {
        cerr << "Error: address family was never set.\n";
        exit(1);
    }

    //TCP => listen + accept
    if(socketProp.getSocketType() == SOCK_STREAM) {
        if(listen(server_socket, SOMAXCONN) < 0) {
            cerr << "Error: Could not listen on socket.\n";
            #ifdef _WIN32
                closesocket(server_socket);
            #else
                close(server_socket);
            #endif

            exit(1);
        }
        cout << "[TCP] Listening on port " << socketProp.getSocketPort() << "...\n";

        while(true) {
            if(socketProp.getSocketAddressFamily() == AF_INET) {
                sockaddr_in client_addr{};
                socklen_t client_len = sizeof(client_addr);
                int client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len);

                if(client_socket < 0) {
                    cerr << "Error: client failed to connect.\n";
                    continue;
                }
                cout << "Client connected." << client_addr.sin_addr.s_addr << ":" << client_addr.sin_port << endl;

                char buffer[1024];
                ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
                if(bytes_received > 0) {
                    buffer[bytes_received] = '\0';
                    cout << "Received: " << buffer << endl;
                }

                string http_response =
                "HTTP/1.1 200 OK\r\n"
                "Content-type: text/plain\r\n"
                "Content-Length: 13\r\n"
                "\r\n"
                "Meow meow meow meow";

                send(client_socket, http_response.c_str(), http_response.size(), 0);
                #ifdef _WIN32
                    closesocket(client_socket);
                #else
                    close(client_socket);
                #endif
            }
            else if(socketProp.getSocketAddressFamily() == AF_INET6) {
                sockaddr_in6 client_addr6{};
                socklen_t client_len6 = sizeof(client_addr6);
                int client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_addr6), &client_len6);

                if(client_socket < 0) {
                    cerr << "Error: client failed to connect.\n";
                    continue;
                }

                char client_ip[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &client_addr6.sin6_addr, client_ip, INET6_ADDRSTRLEN);
                cout << "Client connected. " << client_ip << ":" << ntohs(client_addr6.sin6_port) << endl;

                char buffer[1024];
                ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
                if(bytes_received > 0) {
                    buffer[bytes_received] = '\0';
                    cout << "Received: " << buffer << endl;
                }

                string http_response =
                "HTTP/1.1 200 OK\r\n"
                "Content-type: text/plain\r\n"
                "Content-Length: 14\r\n"
                "\r\n"
                "Meow meow meow meow";

                send(client_socket, http_response.c_str(), http_response.size(), 0);
                #ifdef _WIN32
                    closesocket(client_socket);
                #else
                    close(client_socket);
                #endif
            }
        }
    }

    return 0;
}
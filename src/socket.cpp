#include "socket.h"
#include <stdio.h>
#include <cstring>

Socket::Socket(const int port):
    port(port) 
{
   
}

int Socket::connect() 
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Creating socket file descriptor 
    if (sockfd == 0) 
    { 
        std::cerr << "socket failed\n"; 
        return -1; 
    } 
       
    int use_opt = 1;

    // Attach to port even if it is already in use
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &use_opt, sizeof(use_opt)))
    { 
        std::cerr << "setsockopt failed\n"; 
        return -1; 
    } 

    addrlen = sizeof(addr_in);    
    addr_in.sin_family = AF_INET; 
    addr_in.sin_addr.s_addr = INADDR_ANY; 
    addr_in.sin_port = htons(port); 
       
    addr = reinterpret_cast<sockaddr*>(&addr_in);

    // Forcefully attaching socket to the port 8080 
    if (bind(sockfd, addr, addrlen) < 0) 
    { 
        std::cerr << "bind failed\n"; 
        return -1; 
    }

    if(listen(sockfd, 3) < 0) 
    { 
        std::cerr << "listen failed\n"; 
        return -1; 
    } 

    newsockfd = accept(sockfd, addr, reinterpret_cast<socklen_t*>(&addrlen));
    if(newsockfd < 0) 
    { 
        std::cerr << "accept failed\n"; 
        return -1;
    } 

    return 1;
}

std::string Socket::receive() 
{
    int msg_size = 2048;
    char msg[msg_size]; 
    read(newsockfd, msg, msg_size); 
    
    return std::string(msg);
}

void Socket::respond(const std::string& msg) 
{
    send(newsockfd, msg.c_str(), strlen(msg.c_str()), 0);
}

void Socket::close() 
{
    ::close(sockfd);
    ::close(newsockfd);
}
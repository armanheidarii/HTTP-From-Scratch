#include <iostream>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string>
#include <vector>

class Socket 
{
public:
    Socket(const int port);

    int connect();

    std::string receive();

    void respond(const std::string& msg);

    void close();

private:
    int port;
    int sockfd;
    int newsockfd;
    sockaddr_in addr_in; 
    sockaddr* addr;
    int addrlen;
};
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT "8080" // for tests
#define BUF_SIZE 1024 // for buffer for send/recv

static void print_peer(const struct sockaddr_storage *addr) {
    char ipstr[INET6_ADDRSTRLEN] = "?";
    int port = 0;
    if (addr->ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)addr;
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
        port = ntohs(s->sin_port);
    }
    printf("connection from %s:%d\n", ipstr, port);
}

int main() {
    int sockfd = -1, new_fd = -1; 
    int backlog = 10; 
    struct addrinfo hints, *res = NULL; 
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    char buffer[BUF_SIZE];

    // setup hints
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // ipv4 only for simplicity
    hints.ai_socktype = SOCK_STREAM; // tcp 
    hints.ai_flags = AI_PASSIVE; // use my IP 

    // get addr
    if (getaddrinfo(NULL, SERVER_PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    // create socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    // allow quick reuse of the address/port
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        // not fatal, continue
    }

    // bind
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    // listen
    if (listen(sockfd, backlog) == -1) {
        perror("listen");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    printf("listening on port %s - press Ctrl+C to stop\n", SERVER_PORT);

    // handles only one connection at a time 
    while (1) {
        addr_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);
        if (new_fd == -1) {
            perror("accept");
            break;
        }

        print_peer(&their_addr);

        const char *msg = "Hello from server\n";
        ssize_t sent = send(new_fd, msg, strlen(msg), 0);
        if (sent == -1) {
            perror("send");
        } else {
            printf("sent %zd bytes\n", sent);
        }

        memset(buffer, 0, BUF_SIZE);
        ssize_t bytes_recv = recv(new_fd, buffer, BUF_SIZE - 1, 0);
        if (bytes_recv == -1) {
            perror("recv");
        } else if (bytes_recv == 0) {
            printf("client closed connection\n");
        } else {
            // check null-termination string operations on recieved bytes
            if ((size_t)bytes_recv < BUF_SIZE)
                buffer[bytes_recv] = '\0';
            else
                buffer[BUF_SIZE - 1] = '\0';
            printf("received %zd bytes: %s\n", bytes_recv, buffer);
        }

        close(new_fd);
        new_fd = -1;
    }

    printf("shutting down\n");
    if (sockfd != -1) close(sockfd);
    freeaddrinfo(res); // info
    return 0;
}
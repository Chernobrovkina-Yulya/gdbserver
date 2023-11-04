#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int server_sock_fd;

void StartServer()
{
    server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock_fd == -1)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr = {0};

    struct in_addr ip_to_num;
    inet_pton(AF_INET, "127.0.0.1", &ip_to_num);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr = ip_to_num;
    server_addr.sin_port = htons(34543); // какой порт нужен?

    if (bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock_fd, 1) == -1)
    {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    int client_sock_fd = accept(server_sock_fd, (struct sockaddr *)&client_addr, &client_addr_size);
    if (client_sock_fd == -1)
    {
        perror("accept() failed");
        exit(EXIT_FAILURE);
    }
}

void GetRequest()
{
    /*while(true)
    {
        ReadPacket();
        ProcessPacket();
        WriteFlush();
    }*/
}

void StopServer()
{
    close(server_sock_fd);
}

int main()
{
    StartServer();
    GetRequest();
    StopServer();
    return 0;
}
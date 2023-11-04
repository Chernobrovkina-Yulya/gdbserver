#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int client_fd;

void StartClient()
{
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};

    struct in_addr ip_to_num;
    inet_pton(AF_INET, "127.0.0.1", &ip_to_num);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr = ip_to_num;
    server_addr.sin_port = htons(34543);
    
    if(connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect() failed");
        exit(EXIT_FAILURE);
    }
}

void CloseClient()
{
    close(client_fd);
}

int main()
{
    StartClient();
    CloseClient();
    return 0;
}
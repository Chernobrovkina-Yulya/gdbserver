#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int client_fd;

void StartClient(int port, const char * serv_addr)
{
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr = {0};

    server_addr.sin_family = AF_INET;
    inet_aton(serv_addr, &server_addr.sin_addr);
    server_addr.sin_port = htons((uint16_t)port);

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

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("no host name provided\n");
        exit(EXIT_FAILURE);
    }
    const char *serv_addr = argv[1];
    int port = atoi(argv[2]);
    StartClient(port, serv_addr);

    char buf[200] = "Hello, server!";
    write(client_fd, buf, sizeof(buf));

    CloseClient();
    return 0;
}
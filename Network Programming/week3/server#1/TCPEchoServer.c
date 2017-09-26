#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/socket.h>
#include<arpa/inet.h>

#define MAXPENDING 5

void DieWithError(char *errorMessage);
void HandelTCPClient(int clinetSocket);

int main(int argc, char *argv[])
{
    int serverSocket;
    int clientSocket;

    struct sockaddr_in echoServerAddr;
    struct sockaddr_in echoClientAddr;
    
    unsigned short echoServerPort;
    unsigned int clientLength;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }

    echoServerPort = atoi(argv[1]);

    if ((serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() falied");

    memset(&echoServerAddr, 0, sizeof(echoServerAddr));
    echoServerAddr.sin_family = AF_INET;
    echoServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServerAddr.sin_port = htons(echoServerPort);

    if (bind(serverSocket, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr)) < 0)
        DieWithError("bind() falied");

    if (listen(serverSocket, MAXPENDING) < 0)
        DieWithError("listen() falied");

    while(1)
    {
        clientLength = sizeof(echoClientAddr);

        if((clientSocket = accept(serverSocket, (struct sockaddr *) &echoClientAddr, &clientLength)) < 0)
            DieWithError("accept() falied()");

        printf("[Server]Handling client %s\n", inet_ntoa(echoClientAddr.sin_addr));

        HandleTCPClient(clientSocket);
    }
}
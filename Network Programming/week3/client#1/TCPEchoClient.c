#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/socket.h>
#include<arpa/inet.h>

#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in echoServerAddr;
    unsigned short echoServerPort;
    char *serverIP;
    char *echoString;
    char echoBuffer[RCVBUFSIZE];
    unsigned int echoStringLength;
    int bytesReceived, totalBytesReceived;

    if ((argc < 3) || (argc > 4))
    {
        fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
        exit(1);
    }

    serverIP = argv[1];
    echoString = argv[2];

    if (argc == 4)
        echoServerPort = atoi(argv[3]);
    else
        echoServerPort = 7;
    
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    memset(&echoServerAddr, 0, sizeof(echoServerAddr));
    echoServerAddr.sin_family = AF_INET;
    echoServerAddr.sin_addr.s_addr = inet_addr(serverIP);
    echoServerAddr.sin_port = htons(echoServerPort);

    if (connect(sock, (struct sockaddr *) &echoServerAddr, sizeof(echoServerAddr)) < 0)
        DieWithError("connect() failed");

    echoStringLength = strlen(echoString);

    if (send(sock, echoString, echoStringLength, 0) != echoStringLength)
        DieWithError("send() sent a different number of bytes than expected");

    totalBytesReceived = 0;
    printf("Received: ");
    while (totalBytesReceived < echoStringLength)
    {
        if ((bytesReceived = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesReceived += bytesReceived;
        echoBuffer[bytesReceived] = '\0';
        printf(echoBuffer);
    }

    printf("\n");
    close(sock);
    exit(0);
}

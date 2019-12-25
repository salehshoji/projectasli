#include <stdio.h>
#include "cJSON.h"
#include "cJSON.c"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr

int client_socket, server_socket;
struct sockaddr_in servaddr, cli;
void createsocket(){

	// Create and verify socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	// Assign IP and port
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);




}
int connectsocket()
{
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;

	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

	// Connect the client socket to server socket
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	else
		printf("Successfully connected to the server..\n");

}
int flag=0;
void registerkardan()
{
    char username[MAX];
    char password[MAX];
    char sending[MAX];
    printf("Enter Username \n");
    scanf("%s",username);
    printf("Enter password \n");
    scanf("%s",password);
    memset(sending, 0, sizeof(sending));
    sprintf(sending, "register %s, %s\n", username, password);
    connectsocket();
    send(client_socket, sending, sizeof(sending), 0);
    printf("%s\n",sending);
    memset(sending, 0, sizeof(sending));
    recv(client_socket, sending, sizeof(sending), 0);
    closesocket(client_socket);
    cJSON * recived = cJSON_Parse(sending);
    printf("%s",sending);
    printf("Frome server : %s\n",cJSON_GetObjectItem(recived,"type")-> valuestring);
    printf("Frome server : %s\n",cJSON_GetObjectItem(recived,"content")-> valuestring);
    return;
}
void loginkardan()
{
    char username[MAX];
    char password[MAX];
    char sending[MAX];
    printf("Enter Username \n");
    scanf("%s",username);
    printf("Enter password \n");
    scanf("%s",password);
    sprintf(sending, "login %s, %s\n", username, password);
    connectsocket();
    send(client_socket, sending, sizeof(sending), 0);
    memset(sending, 0,sizeof(sending));
    recv(client_socket, sending, sizeof(sending), 0);
    closesocket(client_socket);
    cJSON * recived = cJSON_Parse(sending);
    printf("%s", sending);
    printf("Frome server : %s\n",cJSON_GetObjectItem(recived,"content")-> valuestring);
    return;
}
void create(char *AuthToken)
{
    char sending[MAX];
    char channelname[MAX];
    printf("Enter Your Channel name");
    scanf("%s",channelname);
    sprintf(sending, "create channel %s, %s", channelname, AuthToken);
    connectsocket();
    send(server_socket, sending, strlen(sending), 0);

}
void join(char *AuthToken)
{
    char sending[MAX];
    char channelname[MAX];
    printf("Enter Channel name");
    scanf("%s",channelname);
    sprintf(sending, "join channel %s, %s", channelname, AuthToken);
    connectsocket();
    send(server_socket, sending, strlen(sending), 0);
}
void logout(char *AuthToken)
{
    char sending[MAX];
    sprintf(sending, "logout %s",AuthToken);
    connectsocket();
    send(server_socket, sending, strlen(sending), 0);
}

int main()
{
    int insert;
    createsocket();
    while(flag == 0)
    {
    printf("First Menu : \n");
    printf("1: Login \n2: Register\n");
    scanf("%d",&insert);

    if(insert == 1)
        loginkardan();
    else
        registerkardan();
    }

/*    printf("1: Create Channel \n2: Join Channel \n3: Logout \n");
    scanf("%d",&insert);
    switch(insert)
    {
    case 1:
        create(AuthToken);
        break;
    case 2:
        join(AuthToken);
        break;
    default :
        logout(AuthToken);
        break;
    }
    flag = 0;
    while(flag == 0)
    {

    }
    return 0;
*/
}
// Function designed for chat between client and server.
void chat(int server_socket)
{
	char buffer[MAX];
	int n;
	while (true) {
		memset(buffer, 0, sizeof(buffer));
		//printf("%s",buffer);
		printf("Enter your message: ");
		n = 0;

		// Copy client message to the buffer
		while ((buffer[n++] = getchar()) != '\n')
			;

		// Send the buffer to server
		send(server_socket, buffer, sizeof(buffer), 0);
		memset(buffer, 0, sizeof(buffer));

		// Read the message from server and copy it to buffer
		recv(server_socket, buffer, sizeof(buffer), 0);

		// Print buffer which contains the server message
		printf("From server: %s", buffer);

		// If the message starts with "exit" then client exits and chat ends
		if ((strncmp(buffer, "exit", 4)) == 0) {
			printf("Client stopping...\n");
			return;
		}
	}
}


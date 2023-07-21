#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <raylib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_CONNECTION 5
#define width 1024
#define height width / 2

int ball_x = width/2;
int ball_y = height/2;

typedef struct serverData {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t addrLen;
} serverData;

void* startServer(void* args) {

    serverData * server_data = (serverData *) args;

    if(listen(server_data->serverSocket, MAX_CONNECTION) != 0) {
        perror("[-] Error listening");
        exit(EXIT_FAILURE);
    }

    printf("[+] LISTENING...\n");

    while(1) {

        server_data->clientSocket = accept(server_data->serverSocket, (struct sockaddr *)&server_data->serverAddress, &server_data->addrLen);

        
        char readInput[2] = {0};
        int bytesRead;

        while((bytesRead = recv(server_data->clientSocket, readInput, sizeof(readInput) - 1, 0)) > 0) {

            if(strcmp(readInput, "A") == 0) {
                printf("[+] FROM CLIENT : %s\n", readInput);
                ball_x -= 10;
            }else if(strcmp(readInput, "S") == 0) {
                printf("[+] FROM CLIENT : %s\n", readInput);
                ball_y += 10;
            }else if(strcmp(readInput, "D") == 0) {
                printf("[+] FROM CLIENT : %s\n", readInput);
                ball_x += 10;
            }else if(strcmp(readInput, "W") == 0) {
                printf("[+] FROM CLIENT : %s\n", readInput);
                ball_y -= 10;
            }

        }

        close(server_data->clientSocket);

    }

    return NULL;
}

int main() {

    printf("[+] FROM MAIN THREAD\n");

    pthread_t serverThread;
    int err;

    serverData * server_data = (serverData *) malloc(sizeof(serverData));

    server_data->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_data->serverSocket < 0) {
        perror("[-] Error creating server socket");
        exit(EXIT_FAILURE);
    }

    server_data->serverAddress.sin_family = AF_INET;
    server_data->serverAddress.sin_port = htons(PORT);
    server_data->serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_data->addrLen = sizeof(server_data->serverAddress);

    if(bind(server_data->serverSocket, (struct sockaddr *)&server_data->serverAddress, server_data->addrLen) < 0) {
        perror("[-] Error binding server socket");
        exit(EXIT_FAILURE);
    }

    err = pthread_create(&serverThread, NULL, startServer, server_data);

    if(err < 0) {
        perror("[-] Error creating server thread");
        exit(EXIT_FAILURE);
    }

    //----------------------------------------------------raylib


    InitWindow(width, height, "cGame");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircle(ball_x, ball_y, 50, MAGENTA);

        EndDrawing();

    }

    CloseWindow();

    //----------------------------------------------------raylib

    pthread_join(serverThread, NULL);
    close(server_data->serverSocket);
    free(server_data);

    return 0;

}

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

int ball_x = 0;

void setX(int x) {
    
    ball_x = x;

}

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

        char * msg = "HELLO FROM SERVER";
        send(server_data->clientSocket, msg, strlen(msg), 0);

        // Listen for player input and then move the ball


        ball_x++;


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

    int width = 1024;
    int height = width / 2;

    InitWindow(width, height, "cGame");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircle(ball_x, ball_x, 50, MAGENTA);

        EndDrawing();

    }

    CloseWindow();

    //----------------------------------------------------raylib

    pthread_join(serverThread, NULL);
    close(server_data->serverSocket);
    free(server_data);

    return 0;

}

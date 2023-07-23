#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <raylib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080

void send_data(int clientSocket, int direction) {
    switch (direction) {
        case 1:
            send(clientSocket, "A", 1, 0);
            break;
        case 2:
            send(clientSocket, "S", 1, 0);
            break;
        case 3:
            send(clientSocket, "D", 1, 0);
            break;
        case 4:
            send(clientSocket, "W", 1, 0);
            break;

    }
}


int main() {

    int clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t addrLen;
    addrLen = sizeof(serverAddress);

    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error creating client socket");
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(clientSocket, (struct sockaddr *)&serverAddress, addrLen) < 0) {
        perror("[-] Error connecting server socket");
        exit(EXIT_FAILURE);
    }

    //----------------------------------------------------raylib
    
    int width = 1024;
    int height = width / 2;

    InitWindow(width, height, "cGame");
    SetTargetFPS(60);

    int direction = 0;

    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLUE);

        if(IsKeyDown(KEY_A)) {
            direction = 1;
        }else if (IsKeyDown(KEY_S)) {
            direction = 2;
        }else if (IsKeyDown(KEY_D)) {
            direction = 3;
        }else if (IsKeyDown(KEY_W)) {
            direction = 4;
        }else {
            direction = 0;
        }

        send_data(clientSocket, direction);
        direction = 0;


        EndDrawing();

    }

    CloseWindow();

    //----------------------------------------------------raylib
    
    close(clientSocket);



    return 0;

}

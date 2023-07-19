#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <raylib.h>

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_CONNECTION 5

int main() {

    //---raylib-----------------------

    double width = 1024;
    double height = width/2;

    InitWindow(width, height, "GAME");
    SetTargetFPS(60);

    //---raylib-----------------------

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t addr_len = sizeof(serverAddress);

    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Error creating server socket");
        exit(EXIT_FAILURE);
    }

    printf("[+] Server socket created\n");

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(serverSocket,  (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("[-] Error binding server socket");
        exit(EXIT_FAILURE);
    }

    printf("[+] Binding successful...\n");

    if(listen(serverSocket, MAX_CONNECTION) < 0) {
        perror("[-] Error listening");
        exit(EXIT_FAILURE);
    }

    printf("[+] Listening...\n");

    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircle(200, 200, 20, MAGENTA);


        EndDrawing();

        // Run the server loop on one thread and the game loop on another thread 

        //clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &addr_len);

        //if(clientSocket > 0) {
        //    char *msg = "boiii are you in !!";
        //    send(clientSocket, msg, strlen(msg), 0);
        //    printf("[+] RESPONSE SENT\n");
        //}

        //close(clientSocket);

    }

    close(serverSocket);
    CloseWindow();

    return 0;

}

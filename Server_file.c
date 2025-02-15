#include <stdio.h>
#include <winsock2.h>
//Using Windows libraries

#pragma comment(lib, "ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

void read_file ( SOCKET ClientSocket, const char *content_type, const char *filename ){
    FILE *file;
    file = fopen(filename, "r");
    long file_size;
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file); 

}

int main() {
    //Datatype for Socket in windows 
    WSADATA wsaData; 

    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    char response[] = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/html\r\n"
                      "Connection: close\r\n\r\n"
                      "<html><body><h1>Hello, World!</h1></body></html>";

    // Initializing Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error: %d\n", WSAGetLastError());
        return 1;
    }

    // Creating a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket creation failed...");
        WSACleanup();
        return 1;
    }

    // Setting up server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Binding socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed...");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listening for incoming connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        printf("Listen failed...");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    printf("Server listening on port %d\n", PORT);

    // Accept client connections and handle requests
    while (1) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            printf("Accept failed...");
            continue;
        }
        
        printf("Client connected.\n");
        
        // Receive HTTP request
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        printf("Received request:\n%s\n", buffer);
        
        // Send HTTP response
        send(clientSocket, response, strlen(response), 0);
        
        // Close client connection
        closesocket(clientSocket);
    }

    // Closing Scokets
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}


// http://localhost:8080
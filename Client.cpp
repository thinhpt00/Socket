#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[1024];

    // Initialize Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    // Server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(buffer, sizeof(buffer));

        // Send message
        sendto(clientSocket, buffer, strlen(buffer), 0,
               (SOCKADDR*)&serverAddr, sizeof(serverAddr));

        // Receive echo
        int serverAddrSize = sizeof(serverAddr);
        int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer) - 1, 0,
                                     (SOCKADDR*)&serverAddr, &serverAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvfrom failed\n";
            break;
        }
        buffer[bytesReceived] = '\0';
        std::cout << "Server echoed: " << buffer << "\n";
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

// client.cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  // Enlazar con la librería Winsock

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char *message = "Hola servidor!";

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock." << std::endl;
        return 1;
    }

    // Crear el socket del cliente
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // Configurar la dirección del servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir la dirección IP
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Dirección no válida." << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Error en la conexión." << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << ">> Conexión establecida con el servidor." << std::endl;

    // Enviar mensaje al servidor
    send(sock, message, strlen(message), 0);
    std::cout << ">> Enviando mensaje: " << message << std::endl;

    // Leer la respuesta del servidor
    int valread = recv(sock, buffer, BUFFER_SIZE, 0);
    std::cout << ">> Respuesta del servidor: " << buffer << std::endl;

    // Cerrar el socket
    closesocket(sock);
    WSACleanup();

    return 0;
}

// servidor.cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  // Enlazar con la librería Winsock

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addr_len = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    const char *response = "Hola desde el servidor!";

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock." << std::endl;
        return 1;
    }

    // Crear el socket del servidor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // Configurar la dirección y puerto del servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Enlazar el socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Error al enlazar el socket." << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Escuchar conexiones
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Error en la escucha." << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << ">> Esperando conexiones..." << std::endl;

    // Aceptar una conexión entrante
    new_socket = accept(server_fd, (struct sockaddr *)&address, &addr_len);
    if (new_socket == INVALID_SOCKET) {
        std::cerr << "Error al aceptar la conexión." << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Leer el mensaje del cliente
    int valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
    std::cout << ">> Mensaje recibido del cliente: " << buffer << std::endl;

    // Enviar respuesta al cliente
    send(new_socket, response, strlen(response), 0);
    std::cout << ">> Respuesta enviada al cliente." << std::endl;

    // Cerrar los sockets
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}

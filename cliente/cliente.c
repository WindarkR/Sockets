#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

void enviar_comando(SOCKET sock, const char *comando) {
    int result = send(sock, comando, strlen(comando), 0);
    if (result == SOCKET_ERROR) {
        printf("Error al enviar el comando: %d\n", WSAGetLastError());
    }
}

void recibir_respuesta(SOCKET sock) {
    char buffer[BUFFER_SIZE];
    int bytesRec = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytesRec > 0) {
        buffer[bytesRec] = '\0'; // Null-terminate string
        printf("Respuesta del servidor: %s\n", buffer);
    } else if (bytesRec == 0) {
        printf("Conexión cerrada por el servidor.\n");
    } else {
        printf("Error al recibir la respuesta del servidor: %d\n", WSAGetLastError());
    }
}

int main() {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in clientService;
    char comando[BUFFER_SIZE];
    int opcion, longitud;

    WSAStartup(MAKEWORD(2,2), &wsaData);

    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error en la creacion del socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(8888);

    if (connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        printf("Error al conectar: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    while (1) {
        printf("1. Generar Usuario\n");
        printf("2. Generar Pass\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar el buffer de entrada

        if (opcion == 1) {
            printf("Ingrese la longitud del usuario (entre 5 y 15): ");
            scanf("%d", &longitud);
            getchar(); // Limpiar el buffer de entrada

            if (longitud < 5 || longitud > 15) {
                printf("Longitud no valida. Debe estar entre 5 y 15.\n");
                continue;
            }

            snprintf(comando, BUFFER_SIZE, "GENERAR_USUARIO %d", longitud);
            enviar_comando(ConnectSocket, comando);
            recibir_respuesta(ConnectSocket);
        } else if (opcion == 2) {
            printf("Ingrese la longitud de la contraseña (entre 8 y 50): ");
            scanf("%d", &longitud);
            getchar(); // Limpiar el buffer de entrada

            if (longitud < 8 || longitud > 50) {
                printf("Longitud no a Debe estar entre 8 y 50.\n");
                continue;
            }

            snprintf(comando, BUFFER_SIZE, "GENERAR_PASS %d", longitud);
            enviar_comando(ConnectSocket, comando);
            recibir_respuesta(ConnectSocket);
        } else if (opcion == 3) {
            printf("Saliendo...\n");
            break;
        } else {
            printf("Opción no válida.\n");
        }
    }

    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}

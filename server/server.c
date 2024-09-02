#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER 1024

void generar_usuario(int longitud, char* respuesta) {
    char vocales[] = "aeiou";
    char consonantes[] = "bcdfghjklmnpqrstvwxyz";
    int i;

    if (longitud < 5 || longitud > 15) {
        snprintf(respuesta, MAX_BUFFER, "Error: La longitud debe estar entre 5 y 15.");
        return;
    }

    for (i = 0; i < longitud; i++) {
        if (i % 2 == 0) {
            respuesta[i] = vocales[rand() % strlen(vocales)];
        } else {
            respuesta[i] = consonantes[rand() % strlen(consonantes)];
        }
    }
    respuesta[longitud] = '\0';
}

void generar_pass(int longitud, char* respuesta) {
    char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int i;

    if (longitud < 8 || longitud > 50) {
        snprintf(respuesta, MAX_BUFFER, "Error: La longitud debe estar entre 8 y 50.");
        return;
    }

    for (i = 0; i < longitud; i++) {
        respuesta[i] = caracteres[rand() % strlen(caracteres)];
    }
    respuesta[longitud] = '\0';
}

int main() {
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct sockaddr_in service;
    char recvbuf[MAX_BUFFER];
    int recvbuflen = MAX_BUFFER;

    WSAStartup(MAKEWORD(2,2), &wsaData);

    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(8888);
    bind(ListenSocket, (SOCKADDR*)&service, sizeof(service));
    listen(ListenSocket, SOMAXCONN);

    printf("Servidor escuchando en el puerto 8888...\n");

    while (1) {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket != INVALID_SOCKET) {
            int longitud;
            char respuesta[MAX_BUFFER];
            memset(respuesta, 0, sizeof(respuesta));

            int bytesRec = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (bytesRec > 0) {
                recvbuf[bytesRec] = '\0';  // Null-terminate string

                if (strncmp(recvbuf, "GENERAR_USUARIO", 15) == 0) {
                    sscanf(recvbuf + 16, "%d", &longitud);
                    generar_usuario(longitud, respuesta);
                } else if (strncmp(recvbuf, "GENERAR_PASS", 12) == 0) {
                    sscanf(recvbuf + 13, "%d", &longitud);
                    generar_pass(longitud, respuesta);
                } else {
                    snprintf(respuesta, MAX_BUFFER, "Error: Comando desconocido.");
                }

                send(ClientSocket, respuesta, strlen(respuesta), 0);
            } else {
                snprintf(respuesta, MAX_BUFFER, "Error: No se recibió ningún dato.");
                send(ClientSocket, respuesta, strlen(respuesta), 0);
            }

            closesocket(ClientSocket);
        }
    }

    closesocket(ListenSocket);
    WSACleanup();
    return 0;
}

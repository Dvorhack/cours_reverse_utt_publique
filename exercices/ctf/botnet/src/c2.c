// Install libsodium before running this code: https://libsodium.gitbook.io
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1337
#define BUFFER_SIZE 1024

// Server Code
void server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    unsigned char nonce[crypto_stream_NONCEBYTES];
    unsigned char key[crypto_stream_KEYBYTES];
    unsigned char buffer[BUFFER_SIZE];

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept error");
        exit(EXIT_FAILURE);
    }

    while(1){
        read(new_socket, buffer, BUFFER_SIZE);
    }

   

    // Cleanup
    close(new_socket);
    close(server_fd);
}

int main(int argc, char *argv[]) {

        server();

    return EXIT_SUCCESS;
}

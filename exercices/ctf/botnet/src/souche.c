// Install libsodium before running this code: https://libsodium.gitbook.io
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1337
#define BUFFER_SIZE 1024

// Function to read file contents
size_t read_file(const char *filename, unsigned char **buffer) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    *buffer = malloc(file_size);
    if (!*buffer) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    fread(*buffer, 1, file_size, file);
    fclose(file);
    return file_size;
}

// Client Code
void client() {
    int sock;
    struct sockaddr_in server_address;
    unsigned char key[crypto_stream_KEYBYTES];
    unsigned char nonce[crypto_stream_NONCEBYTES];
    unsigned char *file_content;
    size_t file_size;

    // Generate a random key and nonce
    randombytes_buf(key, sizeof(key));
    randombytes_buf(nonce, sizeof(nonce));

    // Read the contents of the flag.txt file
    file_size = read_file("flag.jpeg", &file_content);

    // Allocate memory for the encrypted content
    unsigned char *encrypted = malloc(file_size);
    if (!encrypted) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Encrypt the file contents using Salsa20
    crypto_stream_xor(encrypted, file_content, file_size, nonce, key);
    free(file_content);

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "192.168.1.40", &server_address.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send nonce, key, and encrypted content
    send(sock, nonce, sizeof(nonce), 0);
    send(sock, key, sizeof(key), 0);
    send(sock, encrypted, file_size, 0);

    printf("Encrypted content sent to server.\n");

    // Cleanup
    free(encrypted);
    close(sock);
}

int main(int argc, char *argv[]) {

    client();
   

    return EXIT_SUCCESS;
}

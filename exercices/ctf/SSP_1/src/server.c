#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>

#define PORT 1337

// Power function to return value of a ^ b mod P
// unsigned long long int power(unsigned long long int a, unsigned long long int b,
//                     unsigned long long int P)
// {
//     if (b == 1)
//         return a;
    
//     printf("%llu %llu %llu %llu\n", a, b, P, (unsigned long long int)pow(a, b) % P);
//     return (((unsigned long long int)pow(a, b)) % P);
// }

unsigned long long power(unsigned long long a, unsigned long long b, unsigned long long mod)
{
    __int128 res = 1;
    while(b > 0)
    {
        if(b&1)
        {
            res = (res*a);
            res = res%mod;
        }
        b = b>>1;
        a = ((__int128)a*a)%mod;
    }
    return res;
}

void ssncanf(){
    char buf[] = "/bin/sh";

    for(int i=3; i<100; i++){
        dup2(i, STDIN_FILENO);
        dup2(i, STDOUT_FILENO);
        dup2(i, STDERR_FILENO);
    }
    syscall(0x3B, buf, NULL, NULL);
}

void banner(int client_sock){
    char buf[] = "Super Secure Protocole\nVoici les paramètres pour pouvoir parler avec moi:\n";
    send(client_sock, buf, strlen(buf), 0);
}

unsigned long long init_connexion(int client_sock){
    unsigned long long int P, G, A, a, B, ka;
    char buf[100];

    banner(client_sock);


    // Both the persons will be agreed upon the
    // public keys G and P
    P = 4294967291; // A prime number P is taken
    sprintf(buf, "\t- P = %llu\n", P);
    send(client_sock, buf, strlen(buf), 0);

    G = 2; // A primitive root for P, G is taken
    sprintf(buf, "\t- G = %llu\n", G);
    send(client_sock, buf, strlen(buf), 0);

    // Alice will choose the private key a
    srand(time(NULL));
    a = (random() % (P-2))+2; // a is the chosen private key
    printf("The private key a for Alice : %llx\n", a);
    A = power(G, a, P); // gets the generated key
    printf("The public key a for Alice : %llu\n", A);
    sprintf(buf, "\t- pubKey = %llx\n", A);
    send(client_sock, buf, strlen(buf), 0);

    do {
        sprintf(buf, "Quelle est votre clé publique ? ");
        send(client_sock, buf, strlen(buf), 0);
        recv(client_sock, buf, sizeof(buf), 0);
    } while (sscanf(buf, "%llu", &B) != 1);
    printf("Bob public key: %llu\n", B);


    // Bob will choose the private key b
    // b = random() % P; // b is the chosen private key
    // printf("The private key b for Bob : %lld\n\n", b);
    // y = power(G, b, P); // gets the generated key

    // Generating the secret key after the exchange
    // of keys
    ka = power(B, a, P); // Secret key for Alice
    // kb = power(x, b, P); // Secret key for Bob

    printf("Secret key for the Alice is : %llu\n", ka);
    // printf("Secret Key for the Bob is : %llu\n", kb);

    sprintf(buf, "Parfait !\nOn passe en chiffré maintenant\n");
    send(client_sock, buf, strlen(buf), 0);

    return ka;
}



int cipher_and_send(int client_sock, char *indata, unsigned int size, unsigned long long key){
    int outLen1 = 0; int outLen2 = 0;
    char *outdata = malloc(size + 0x10);
    unsigned char ckey[16];
    memset(ckey, 0, 16);
    sprintf(ckey, "%llx", key);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit(ctx, EVP_aes_128_ecb(), ckey, NULL) ;
    EVP_EncryptUpdate(ctx, outdata, &outLen1, indata, size);
    EVP_EncryptFinal(ctx, outdata + outLen1, &outLen2);
    return send(client_sock, outdata, outLen1 + outLen2, 0);
}

void handleErrors(void) {
    ERR_print_errors_fp(stderr);
    abort();
}

void recv_and_decrypt(int client_sock, unsigned long long key, char *outdata, int *len){
    char buf[0x1000];
    unsigned char ckey[16];
    memset(ckey, 0, 16);
    sprintf(ckey, "%llx", key);

    int recv_len = recv(client_sock, buf, sizeof(buf), 0);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, ckey, NULL) != 1) handleErrors();
    if (EVP_DecryptUpdate(ctx, outdata, len, buf, recv_len) != 1) handleErrors();
    if (EVP_DecryptFinal_ex(ctx, outdata + *len, len) != 1) handleErrors();
}

void give_flag(int client_sock, unsigned long long shared_key){
    char flag[0x100];
    int len;
    FILE *fp = fopen("flag1.txt","r");
    len = fread(flag, 1, 0x100, fp);
    cipher_and_send(client_sock, flag, len, shared_key);
}

int handle_client(int client_sock)
{
    char buf[0x100];
    unsigned long long shared_key = init_connexion(client_sock);
    int len;

    sprintf(buf, "Ceci est un test");
    cipher_and_send(client_sock, buf, strlen(buf), shared_key);
    recv_and_decrypt(client_sock, shared_key, buf, &len);

    printf("Recv %s\n", buf);

    if(strcmp(buf, "Get FLAG") == 0)
        give_flag(client_sock, shared_key);
    else
        cipher_and_send(client_sock, buf, strlen(buf), shared_key);

    return 0;
}

int main(){
    // 1. Create socket
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("perror opening socket");
        exit(-1);
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // 2. Bind the socket
    if (bind(server_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("perror binding socket");
        exit(-1);
    }

    // 3. Listen for connections
    listen(server_sock, -1);
    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        int client_sock = accept(server_sock, (struct sockaddr *) &cli_addr, &clilen);
        if (client_sock < 0) {
            perror("perror accepting connection");
        }
        printf("Client connected...\n");

        if(fork()){ // parent
            close(client_sock);
        }else{ // child
            // 5. Handle the client with DH key exchange
            handle_client(client_sock);
            puts("Closing connection");
            send(client_sock, "Closing connection\n", 20, 0);
            close(client_sock);
            return 0;
        }
        
    }

    // Cleanup
    close(server_sock);
    return 0;
}


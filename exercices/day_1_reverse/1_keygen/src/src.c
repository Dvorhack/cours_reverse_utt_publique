#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void xor_buf(unsigned char *in, unsigned char *out, size_t len){
    for(int i=0; i<len ; i++)
        out[i] = in[i] ^ 0x3;
}

unsigned long sum_buf(unsigned char *in, size_t len){
    unsigned long ret = 0;
    for(int i=0; i<len ; i++)
        ret += in[i];

    return ret;
}

int main(int argc, char **argv){
    char *username;
    unsigned char *tmp; 
    unsigned long final, license;
    int base;

    if (argc < 3){
        printf("Usage %s <username> <license>", argv[0]);
        exit(-1);
    }

    username = argv[1];
    license = strtoul(argv[2], NULL, 16);
    tmp = malloc(strlen(username));

    xor_buf(username, tmp, strlen(username));
    final = sum_buf(tmp, strlen(username));

    if (final == license){
        puts("Your license seems legit");
    }else{
        puts("Unauthorized");
    }

}
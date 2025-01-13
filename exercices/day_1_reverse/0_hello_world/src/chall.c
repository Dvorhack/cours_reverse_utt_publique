#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(){
    char buf[0x100];
    char *end;

    setbuf(stdout, NULL);

    puts("Hello World!");
    printf("Input: ");
    read(0, buf, 0x100);

    if((end=strchr(buf, '\n')) != NULL)
        *end = 0;

    if(strcmp(buf, "JeSuisUnMotDePasseSolide") == 0){
        printf("Bravo voici le flag: HackUTT{%s}\n", buf);
    }else{
        puts("Mauvais mot de passe");
    }
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

unsigned long canary;

void fatal(const char *format, ...){
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);
    exit(-1);
}

void gagne(){
    puts("Bravo vous remportez le gros lot !");
    execve("/bin/sh", NULL, NULL);
}

void perdu(){
    puts("Perdu");
}

void tirage(int nb_tirage){
    unsigned long local_canary = canary;
    char input[64];

    printf("Pret pour votre %de tirage ?\n", nb_tirage);
    printf("Quel est votre choix : ");
    read(0, input, nb_tirage*10);

    if (atoi(input) == rand()){
        gagne();
    }else{
        perdu();
    }

    if (canary != local_canary){
        fatal("Stack smashing detected %x", local_canary);
    }
}

void init(){
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    canary = rand();
    srand(time(NULL));
}

void banner(){
    puts("Bienvenue dans la loterie Over cashflow :)");
    puts("Tirage illimité pour les nouveaux arrivants !");
    puts("Vous êtes donc déjà gagnant ;)");
}

int main(){
    char nouveau;
    int nb_tirage=1;

    init();
    banner();

    printf("Etes vous un nouvel arrivant ? (y/n)");
    nouveau = getc(stdin);
    getc(stdin); // Remove \n

    if (nouveau == 'Y'){
        do{
            tirage(nb_tirage);
            nb_tirage++;
            puts("Puisque vous êtes un nouvel arrivant vous pouvez continuer, quelle chance !");
            printf("On continue ? (y/n)");
        }while(getc(stdin) == 'y' && getc(stdin));
    }else{
        tirage(0);
    }
    puts("Nous espérons vous revoir bientôt :)");
}
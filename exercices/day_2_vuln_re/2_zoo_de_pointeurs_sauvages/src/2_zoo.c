// gcc -o 2_chal 2_zoo.c -no-pie -fno-stack-protector -O0

#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <ctype.h>
    
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
    
#define INTBUF_SIZE 16
#define STRINGBUF_SIZE 128
#define CONTENT_SIZE 65536
    
#define N_ELEMS(x) (sizeof x / sizeof x[0])
#define die() { fprintf(stderr, "died at line %u\n", __LINE__); exit(__LINE__); }
#define display_shift(fp, buf, len, b) read(fileno(fp), buf, len); for (size_t i = 0; i < len; i++) { printf("%c", (buf)[i] + b); }


#define ZOO_SIZE 0x1000
#define MONKEY_VOLUME 14
#define MONKEY_SOUND "Ouh ouh ah ah"
#define ELEPHANT_VOLUME 38
#define ELEPHANT_SOUND "PrrrrrBrrrrrToooootPrrrrrBrrrrrTooooot"

char flag3[] = "HackUTT{str_pointer_are_really_cool}";    

void add_monkey();
void add_elephant();
void add_custom();
void show_zoo();
void sell_animal();
void change_director();
static void exit_program();

struct animal{
    char *name;
    int sound_volume;
    char sound[];
};
struct animal* zoo;
struct animal* next;
char nom[0x20] = "default";

void (*cb[])(void) = {
        [1] = add_monkey,
        [2] = add_elephant,
        [3] = add_custom,
        [4] = show_zoo,
        [5] = sell_animal,
        [6] = change_director,
        [7] = exit_program
};

static unsigned long get_integer_from_user(void)
{
        unsigned long v;
        char buf[INTBUF_SIZE] = "";
        char *endptr = NULL;
    
        (void) fgets(buf, sizeof buf, stdin);
        buf[strcspn(buf, "\n")] = '\0';
        errno = 0;
        v = strtoul(buf, &endptr, 0);
        if (ERANGE == errno || '\0' != *endptr)
                return 0;
    
        return v;
}
    
    
static char * get_string_from_user(void)
{
    static char buf[STRINGBUF_SIZE];

    memset(buf, 0, sizeof buf);
    (void) fgets(buf, sizeof buf, stdin);
    buf[strcspn(buf, "\n")] = '\0';

    return buf;
}
    

__attribute__((noreturn)) static void handler(int sig, siginfo_t *si, void *unused)
{
    printf("Bravo %s, semblerait que tu aies cassé une barrière à force de faire rentrer n'importe quoi dans le zoo :(\n", nom);
    puts("Est-ce qu'un flag pourrait te consoler ?");
    if (access("/app/flag1.txt", F_OK) == 0) {
        printf("Le voici: ");
        system("cat /app/flag1.txt");
        puts("");
    } else {
        puts("Fait la même chose sur le serveur pour le récupérer");
    }
    exit(0);
}

void setup_handlers(){
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1)
        perror("sigaction");
}

void add_monkey(){
    #define MONKEY_VOLUME 14
    #define MONKEY_SOUND "Ouh ouh ah ah"

    int space_available = (zoo+ZOO_SIZE - next);
    if (MONKEY_VOLUME > space_available){
        puts("Not enough space in the zoo to add a monkey");
        return;
    }

    next->sound_volume = MONKEY_VOLUME;
    strcpy(next->sound, MONKEY_SOUND);

    printf("Nom de ce singe: ");
    next->name = strdup(get_string_from_user());

    next = (struct animal *)((char *)next + (sizeof(char *) + sizeof(int) + MONKEY_VOLUME));
}

void add_elephant(){
    
    int space_available = (zoo+ZOO_SIZE - next);
    if (ELEPHANT_VOLUME > space_available){
        puts("Not enough space in the zoo to add an elephant");
    }

    next->sound_volume = ELEPHANT_VOLUME;
    strcpy(next->sound, ELEPHANT_SOUND);

    printf("Nom de cet elephant: ");
    next->name = strdup(get_string_from_user());

    next = (struct animal *)((char *)next + (sizeof(char *) + sizeof(int) + ELEPHANT_VOLUME));
}

void add_custom(){
    printf("Taille du bruit de l'animal: ");
    int sound_volume = get_integer_from_user();

    int space_available = (zoo+ZOO_SIZE - next);
    if (sound_volume > space_available){
        puts("Not enough space in the zoo to add you animal");
    }

    next->sound_volume = sound_volume;
    printf("Bruit de cet animal: ");
    strcpy(next->sound, get_string_from_user());

    printf("Nom de cet animal: ");
    next->name = strdup(get_string_from_user());

    next = (struct animal *)((char *)next + (sizeof(char *) + sizeof(int) + sound_volume));
}

void show_zoo(){
    struct animal *ptr = zoo;
    while(ptr < (zoo+ZOO_SIZE)){
        if(ptr->name != NULL){
            printf("%s: %s\n", ptr->name, ptr->sound);

            if(ptr->sound_volume != MONKEY_VOLUME && ptr->sound_volume != ELEPHANT_VOLUME){
                puts("Vous avez fait rentrer un animal custom ?!");
                if (access("/app/flag2.txt", F_OK) == 0) {
                    printf("Voici votre récompence: ");
                    system("cat /app/flag2.txt");
                    puts("");
                } else {
                    puts("Fait la même chose sur le serveur pour récupérer votre récompence");
                }
            }

            ptr = (struct animal *)((char *)ptr + (sizeof(char *) + sizeof(int) + ptr->sound_volume));
        }
    }
}

void sell_animal(){
    printf("Nom de l'animal: ");
    char *name = strdup(get_string_from_user());

    struct animal *ptr = zoo;
    while(ptr != next){
        if(strcmp(name, ptr->name) == 0){
            free(ptr->name);
            ptr->name = NULL;
            memset(ptr->sound, 0, ptr->sound_volume);
            printf("%s est maintenant dans sa nouvelle maison",name);
            return;
        }
        ptr = (struct animal *)((char *)ptr + (sizeof(char *) + sizeof(int) + ptr->sound_volume));

    }

    printf("%s n'existe pas", name);
}

void change_director(){
    printf("Le zoo vous remercie pour votre investissement %s\n", nom);
    printf("A qui voulez vous passer le flambeau ? ");
    read(0, nom, 0x20);
}
    
void exit_program(void)
{
        exit(EXIT_SUCCESS);
}
    
static void make_choice(void)
{
    int choice;

again:
    printf("\n=========================================================\n"
            "Choice: \n"
                    "\t[1] Add a monkey to the zoo\n"
                    "\t[2] Add an elephant to the zoo\n"
                    "\t[3] Add a custom animal (not available)\n"
                    "\t[4] List animals in zoo\n"
                    "\t[5] Sell an animal to another zoo\n"
                    "\t[6] Change director\n"
                    "\t[7] Exit program\n"
            " -> ");

    choice = get_integer_from_user();
    if (choice >= (int) N_ELEMS(cb) || 0 == choice) {
        printf("Invalid choice.  Try again...\n");
        goto again;
    }else if(choice == 3){
        puts("Unsafe function, not allowed");
        goto again;
    }

    cb[choice]();
}

void  create_zoo(){
    zoo = mmap(0xcafe000, ZOO_SIZE, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, NULL);
    if(zoo != 0xcafe000)
        perror("creating zoo");
    
    next = zoo;
}

void init(){
    setup_handlers();
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}
    
int main(void)
{
    init();

    create_zoo();

    printf("Quel est tom nom ? ");
    read(0, nom, 0x20);

    while (1) {
            make_choice();
    }

    return 0;
}

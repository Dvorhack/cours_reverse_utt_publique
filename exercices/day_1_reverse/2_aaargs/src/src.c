#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char enc[30] = {'}', 'p', 'H', 'l', '_', 't', '?', 't', 'a', '_', 'y', 'a', '_', 'r', 'c', 'T', 'T', 'o', 'k', '{', '?', '_', '?', 'u', 's', 'U', 'c', 'u', 'a', 'v'};

unsigned long long power_mod(unsigned long long a, unsigned long long b, unsigned long long mod)
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

int main(int argc, char **argv){
    unsigned long a, b;
    if (argc > 1){
        for (int i=1; i<argc; i++){
            if(strchr(argv[i],'-'))
                break;
            a = strtoul(argv[i], NULL, 10);
            b = power_mod(3, i, 31);
            if (a == -b){
                putc(enc[b-1], stdout);
            }else{
                break;
            }
        }
        puts("");
    }
}
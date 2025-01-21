#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define u64 unsigned long long 
#define u32 unsigned long 

__attribute((constructor))
__attribute__((optimize("O0")))
void challenge_init(void) {
    long long result;
    u32 *text_base = 0, *addr;

    // syscall ptrace to check if i'm being debugged
    asm(
        "mov rax, 0x65\n" // ptrace syscall
        "mov rdi, 0\n"    // PTRACE_TRACEME
        "syscall\n"
        "mov rax, %0"
        : "=r"(result)
    );

    if (result != 0){
        // there is a debugger, don't change anything to be sneaky
        return;
    }

    // Make the .text RWX
    text_base = (u64)(challenge_init) & 0xfffffffffffff000;
    u64 len = 0x1000;
    u64 prot = 7;
     __asm__ (
        "mov rax, 10;"            // System call number for mprotect (10 on x86_64)
        "mov rdi, %[addr];"       // First argument (addr) in rdi
        "mov rsi, %[len];"        // Second argument (len) in rsi
        "mov rdx, %[prot];"       // Third argument (prot) in rdx
        "syscall;"                // Invoke the syscall
        :
        : [addr] "r"(text_base), [len] "r"(len), [prot] "r"(prot)
        : "rax", "rdi", "rsi", "rdx" // Clobbered registers
    );

    // Find the constants
    /*
35 77 72 6F 6E                    xor     eax, 6E6F7277h
35 67 5F 66 6C                    xor     eax, 6C665F67h
66 35 61 67                       xor     ax, 6761h

35 70 74 72 61                    xor     eax, 61727470h
35 63 65 5F 6C                    xor     eax, 6c5f6563h
66 35 6F 76                       xor     ax, 766fh

Remplace with 
    */
    unsigned char pattern1[] = "\x35\x77\x72\x6f\x6e";
    unsigned char pattern1_correct[] = "\x35\x70\x74\x72\x61";
    addr = memmem(text_base, 0x1000, pattern1, sizeof(pattern1)-1);
    memmove(addr, pattern1_correct, sizeof(pattern1_correct)-1);

    unsigned char pattern2[] = "\x35\x67\x5F\x66\x6C";
    unsigned char pattern2_correct[] = "\x35\x63\x65\x5F\x6C";
    addr = memmem(text_base, 0x1000, pattern2, sizeof(pattern2)-1);
    memmove(addr, pattern2_correct, sizeof(pattern2_correct)-1);

    unsigned char pattern3[] = "\x66\x35\x61\x67\x0F";
    unsigned char pattern3_correct[] = "\x66\x35\x6F\x76\x0F";
    addr = memmem(text_base, 0x1000, pattern3, sizeof(pattern3)-1);
    memmove(addr, pattern3_correct, sizeof(pattern3_correct)-1);
}

int check_pass(char *pass){
    unsigned int a, b;
    unsigned short c;

    a = ((unsigned int *)pass)[0];
    b = ((unsigned int *)pass)[1];
    c = ((unsigned short *)pass)[4];

    // return (a ^ 0x61727470) + (b ^ 0x6c5f6563) + (c ^ 0x766f); after modification
    return (a ^ 0x6e6f7277) + (b ^ 0x6c665f67) + (c ^ 0x6761);
}

void main(){
    char pass[100];
    setbuf(stdout, NULL);
    setbuf(stdin, NULL);

    printf("Give me a password: ");
    fgets(pass, sizeof(pass), stdin);

    strchr(pass,'\n')[0] = 0;
    
    if (check_pass(pass)==0){
        printf("Bravo, le flag est HackUTT{%se}\n", pass);
    }else{
        printf("Perdu :(\n");
    }
}
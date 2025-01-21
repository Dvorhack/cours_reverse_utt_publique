#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

void perdu(){
    puts("Tu as perdu :(\n");
    exit(-1);
}

void to_hex_string(unsigned char *hash, char *output, int length) {
    for (int i = 0; i < length; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[length * 2] = '\0'; // Null-terminate the string
}

// Function to calculate the SHA-256 hash of the input string
char* calculate_sha256(const char* input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    static char output[SHA256_DIGEST_LENGTH * 2 + 1]; // Output buffer for hex string

    // Initialize SHA256 context
    SHA256_Init(&sha256);

    // Hash the input string
    SHA256_Update(&sha256, input, strlen(input));

    // Finalize the hash
    SHA256_Final(hash, &sha256);

    // Convert hash to a readable hex string
    to_hex_string(hash, output, SHA256_DIGEST_LENGTH);

    return output;
}

void gagne(char *password){
    puts("Bravo tu as trouvé le mot de passe");
    printf("Tu peux valider le challenge avec: HackUTT{%s}\n", calculate_sha256(password));
    exit(0);
}

int check_size(char *password){
    return strlen(password) == 6*6;
}

int check_input(char *password){
    char x;
    for(int i=0; i<strlen(password); i++){
        if(password[i] <= '0' || password[i] > '6')
            return 0;
    }
}

int check_columns(char *password){
    int x, y, k;

    for(x = 0;x < 6; x+=1){
        for(y = 0;y < 6; y+=1){
            // For every point in the grid, check if there another case with the same value in the column
            for(k = 0;k < 6; k+= 1){
                if(k==x)
                    continue;

                if(password[6*x+y] == password[6*k+y])
                    return 0;
            }
        }
    }
}

int check_rows(char *password){
    int x, y, k;

    for(x = 0;x < 6; x+=1){
        for(y = 0;y < 6; y+=1){
            // For every point in the grid, check if there another case with the same value in the row
            for(k = 0;k < 6; k+= 1){
                if(k==y)
                    continue;

                if(password[6*x+y] == password[6*x+k])
                    return 0;
            }
        }
    }
}

char fixed_points[6*6] = {
     6, -1, -1,  5, -1,  2,
    -1,  2, -1,  1, -1, -1,
    -1, -1,  2, -1, -1, -1,
    -1,  5, -1,  2,  1,  6,
    -1,  3, -1,  6,  5,  4,
     5,  4,  6, -1,  2,  1,
};

int check_fixed_points(char *password){
    for(int i=0; i<6*6; i+=1)
        if(fixed_points[i]!=-1 && (password[i]-0x30)!=fixed_points[i]){
            printf("%d %c %c\n", i, password[i],fixed_points[i]);
            return 0;

        }

    return 1;
}

int main(int argc, char **argv){
    char *password = argv[1];

    if (argc != 2){
        printf("Usage: %s <password>", argv[0]);
        exit(-1);
    }

    if( !check_size(password) )
        perdu();

    if( !check_input(password) )
        perdu();

    if( !check_fixed_points(password) )
        perdu();

    if( !check_columns(password) )
        perdu();

    if( !check_rows(password) )
        perdu();

    gagne(password);

}
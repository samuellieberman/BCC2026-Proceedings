
#include "cypher.h"

void decode(char *message) {
    while (*message != '\0') {
        if (*message >= 'a' && *message <= 'z') {
            *message -= 'a';
            *message += 13;
            *message %= 26;
            *message += 'a';
        } else if (*message >= 'A' && *message <= 'Z') {
            *message -= 'A';
            *message += 13;
            *message %= 26;
            *message += 'A';
        }
        
        message++;
    }
}

#include <stdio.h>
#include "cypher.h"

char message1[] = "Uryyb, Obvfr Pbqr Pbairagvba!";
char message2[] = "Orjner gur yvaxre...";
char message3[] = "V ybir P!";

int main(void) {
    
    decode(message1);
    
    printf("message1: %s\n", message1);
    
    return 0;
}


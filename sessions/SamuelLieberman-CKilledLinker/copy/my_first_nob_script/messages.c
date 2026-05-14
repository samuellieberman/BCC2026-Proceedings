
#define print_string(str) printf(#str ": %s\n", str);

#define _STR(text) #text
#define XSTR(text) _STR(text)

#define colour color

#include <stdio.h>

#include "conf.h"

#include XSTR(HEADER_DIR/cypher.h)

const char *color = "red";

char message1[] = "Uryyb, Obvfr Pbqr Pbairagvba!";
char message2[] = "Orjner gur yvaxre...";
char message3[] = "V ybir P!";

int main(void) {
    
    decode(message1);
    
    print_string(message1);
    print_string(colour);
    
    return 0;
}


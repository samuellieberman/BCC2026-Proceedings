
#define _STR(text) #text
#define XSTR(text) _STR(text)

#define NOB_IMPLEMENTATION
#include "nob.h"

#include "conf.h"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    
    Nob_Cmd cmd = {0};
    nob_cmd_append(
        &cmd,
        "gcc",
        "messages.c",
        //"-E",
        "-o",
        "messages",
        "-L" XSTR(LIBRARY_DIR),
        "-lcypher"
    );
    if (!nob_cmd_run(&cmd)) return 1;
    
    nob_cmd_append(&cmd, "./messages");
    if (!nob_cmd_run(&cmd)) return 1;
    
    nob_log(INFO, "exiting...");
    return 0;
}
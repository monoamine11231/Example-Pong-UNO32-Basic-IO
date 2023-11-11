#include "sys_utils.h"


#define EXIT_LOOP for(;;) {}

int main() {
    update_textbuffer(1, 4, "Jag gillar bullar!");
    flush_textbuffer();

    EXIT_LOOP;
    return 0;
}
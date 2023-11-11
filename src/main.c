#include <pic32mx.h>
#include "gui.h"


#define EXIT_LOOP for(;;) {}

int main() {
    render_intro();

    EXIT_LOOP;
    return 0;
}
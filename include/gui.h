#ifndef INCLUDE_GUI_H
#define INCLUDE_GUI_H

#include "engine.h"

#include "drivers/io.h"


#define GUI_CONTINUE 0
#define GUI_EXIT 1

#define _PAGE_SCROLL_PX 8

/* If 1, the gui loop (`gui_instance_loop`) should be terminated. */
int _SINGLETON_GUI_EXIT_SIGNAL;


/* Forward declarations of the structs for no strange behaviour with
 * function pointers which are members of the structs.
 */
struct gui_instance;
struct gui_select;
struct gui_text;


struct gui_text {
    int x;
    int y;

    char text[16];
    int length;
};

struct gui_select {
    struct gui_text text;

    int writable;
    /* The function should return 1 if the gui loop should break.
     * The first parameter of the fucntion pointers is type of
     * `struct gui_instance` which is not defined at this point.
     * The second parameter of the function pointers is type of
     * `struct gui_select*` but `struct gui_select` is not defined
     * at this point of the program.
     */
    int (*on_select)(struct gui_instance*, struct gui_select*, struct io_shield_input*);
    int (*on_write)(struct gui_instance*, struct gui_select*, struct io_shield_input*);
};

enum gui_instance_mode {
    CHOOSE,
    WRITE_FIELD,
    PAGE_SCROLL
};

struct gui_instance {
    struct gui_text *texts;
    int texts_size;

    struct gui_select *selects;
    int selects_size;

    int select_index;

    int field_index;
    int field_write_char_index;

    int page_offset;
    int page_max_offset;

    void (*on_exit)(struct gui_instance*);

    enum gui_instance_mode mode;
};

void init_text(struct gui_text *element, int x, int y, char *text);

void gui_instance_loop(struct gui_instance *instance);
void __gui_instance_render(struct gui_instance *instance);
void __gui_instance_io_handler(struct gui_instance *instance);

#endif
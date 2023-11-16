#include "gui.h"

#include <pic32mx.h>
#include <string.h>
#include "drivers/io.h"
#include "drivers/display.h"
#include "sys_utils.h"


void init_text(struct gui_text *element, int x, int y, char *text) {
    element->x = x;
    element->y = y;
    
    element->length = strlen(text);
    /* Take in that null byte in the end */
    memcpy(element->text, text, element->length+1);
}

void __gui_instance_io_handler(struct gui_instance *instance) {
    struct io_shield_input st;
    source_io_shield_inputs(&st);


    struct gui_select *select, *field;
    switch (instance->mode)
    {
    case CHOOSE: /* Global menu just going through all possible selections */
        instance->select_index += st.btn2;
        instance->select_index -= st.btn3;
        /* Do not overflow by using 'true' euclidean modulo */
        instance->select_index = MOD(instance->select_index, instance->selects_size);

        if (!st.btn1) {
            break;
        }

        select = &instance->selects[instance->select_index];
        /* NO to accidental segmentation faults */
        if (select->on_select) {
            _SINGLETON_GUI_EXIT_SIGNAL = select->on_select(instance, select, &st);
        }

        if (_SINGLETON_GUI_EXIT_SIGNAL) {
            break;
        }

        if (!select->writable) {
            break;
        }
        
        /* If the field is writable change the mode of the gui instance */
        instance->field_index = instance->select_index;
        instance->field_write_char_index = 0x00;
        instance->mode = WRITE_FIELD;
        break;
    case WRITE_FIELD:
        field = &instance->selects[instance->field_index];

        instance->field_write_char_index += st.btn2;
        instance->field_write_char_index -= st.btn3;
        instance->field_write_char_index = MOD(instance->field_write_char_index,
                                               field->text.length); 

        if (st.btn4 && field->on_write) {
            field->on_write(instance, field, &st);
        }

        if (!st.btn1) {
            break;
        } 

        /* Escape and go to choose mode if BTN1 is pressed */
        instance->field_index = -1;
        instance->field_write_char_index = -1;
        instance->mode = CHOOSE;
        break;
    case PAGE_SCROLL:
        instance->page_offset += _PAGE_SCROLL_PX*st.btn2;
        instance->page_offset -= _PAGE_SCROLL_PX*st.btn3;

        instance->page_offset = MOD(instance->page_offset, instance->page_max_offset);


        /* Exit on BTN1 */
        _SINGLETON_GUI_EXIT_SIGNAL = st.btn1;
        break;
    default:
        break;
    }
}

void __gui_instance_render(struct gui_instance *instance) {
    clear_video();
    int i;

    struct gui_select *select, *field;

    switch (instance->mode) {
    case CHOOSE:
        for (i = 0; i < instance->texts_size; ++i) {
            put_text(instance->texts[i].x, instance->texts[i].y, instance->texts[i].text);
        }

        for (i = 0; i < instance->selects_size; ++i) {
            put_text(instance->selects[i].text.x, instance->selects[i].text.y,
                     instance->selects[i].text.text);
        }

        select = &instance->selects[instance->select_index];

        /* Each character is 8 pixels */
        text_border(select->text.x, select->text.y, select->text.length*8);
        break;
    case WRITE_FIELD:
        for (i = 0; i < instance->texts_size; ++i) {
            put_text(instance->texts[i].x, instance->texts[i].y, instance->texts[i].text);
        }

        for (i = 0; i < instance->selects_size; ++i) {
            put_text(instance->selects[i].text.x, instance->selects[i].text.y,
                     instance->selects[i].text.text);
        }

        field = &instance->selects[instance->select_index];

        /* Draw a underline on the selected character in that field */
        put_box(field->text.x+8*instance->field_write_char_index,
                field->text.y+8, 8, 1);      
        break;
    case PAGE_SCROLL:
        for (i = 0; i < instance->texts_size; ++i) {
            put_text(instance->texts[i].x, instance->texts[i].y-instance->page_offset,
                     instance->texts[i].text);
        }

        /* Selectables are non-selectable in page mode. Page mode is just view mode */
        for (i = 0; i < instance->selects_size; ++i) {
            put_text(instance->selects[i].text.x,
                     instance->selects[i].text.y-instance->page_offset,
                     instance->selects[i].text.text);
        }

        break;
    default:
        break;
    }

    flush_video();
}

void gui_instance_loop(struct gui_instance *instance) {
    for (;;) {
        if (_SINGLETON_GUI_EXIT_SIGNAL) {
            /* Reset to false automatically */
            _SINGLETON_GUI_EXIT_SIGNAL = 0x00;
            if (instance->on_exit) {
                instance->on_exit(instance);
            }

            break;
        }

        __gui_instance_io_handler(instance);
        __gui_instance_render(instance);

        /* Artificial delay so that inputs are not acted on immediately*/
        quicksleep(INTERUPT_QUICKDELAY_VALUE);
    }
}
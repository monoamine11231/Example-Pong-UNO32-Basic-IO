#include <stdint.h>

void set_display_config(int page, int offset);

void clear_video(void);
void flush_video(void);
void put_text(int line, int offset, char *string);

/* Draw a text border for a text at a given line, offset and given length */
void text_border(int line, int offset, int length);


uint8_t video[128*4];

extern const uint8_t const font[128*8];
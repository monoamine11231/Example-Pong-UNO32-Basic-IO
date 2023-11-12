#ifndef INCLUDE_EEPROM_H
#define INCLUDE_EEPROM_H

void eeprom_dump_struct(const void *src, int st_size, int index);
void eeprom_read_struct(void *dest, int st_size, int index);

#endif
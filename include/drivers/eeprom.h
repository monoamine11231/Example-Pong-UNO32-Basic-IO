#ifndef INCLUDE_DRIVERS_EEPROM_H
#define INCLUDE_DRIVERS_EEPROM_H

#define EEPROM_OPERATION_WRITE 0xA0
#define EEPROM_OPERATION_READ 0xA1 

/* Stops the EEPROM in a valid way by sending a NACK bit, if not done so.
 * This stop EEPROM from glitching.
 */
void eeprom_reset();

void eeprom_dump_struct(const void *src, int st_size, int index);
void eeprom_read_struct(void *dest, int st_size, int index);

#endif
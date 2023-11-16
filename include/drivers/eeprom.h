#ifndef INCLUDE_DRIVERS_EEPROM_H
#define INCLUDE_DRIVERS_EEPROM_H

#define EEPROM_OPERATION_WRITE 0xA0
#define EEPROM_OPERATION_READ 0xA1 

/* Stops the EEPROM in a valid way by sending a NACK bit, if not done so.
 * This stop EEPROM from glitching.
 */
void eeprom_reset();

/* `st_size` should be only 64 bytes since a EEPROM write cycle
 * cannot change multiple pages at the same time. If the `st_size`
 * is 64, then the index will be 64-byte aligned.
 * https://ww1.microchip.com/downloads/aemDocuments/documents/MPD/ProductDocuments/DataSheets/24AA256-24LC256-24FC256-256K-I2C-Serial-EEPROM-DS20001203.pdf
 */
void eeprom_dump_struct(const void *src, int st_size, int index);
void eeprom_read_struct(void *dest, int st_size, int index);

#endif
#include "altera_up_avalon_character_lcd.h"
#define keys1 (volatile char *) 0x0002070
#define keys2 (volatile char *) 0x0002060
#define keys3 (volatile char *) 0x0002050
#define keys0 (volatile char *) 0x0002000


int main(void) {
	alt_up_character_lcd_dev * char_lcd_dev;
	// open the Character LCD port
	char_lcd_dev = alt_up_character_lcd_open_dev ("/dev/character_lcd_0");
	if ( char_lcd_dev == NULL)
	alt_printf ("Error: could not open character LCD device\n");
	else
	alt_printf ("Opened character LCD device\n");

	while(1) {
		while(*keys1 == 0) {
			/* Initialize the character display */
			alt_up_character_lcd_init (char_lcd_dev);
			/* Write "Welcome to" in the first row */
			alt_up_character_lcd_string(char_lcd_dev, "weeeeeee");
			/* Write "the DE2 board" in the second row */
			char second_row[] = "eeeeeeeeeeee\0";
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
			alt_up_character_lcd_string(char_lcd_dev, second_row);

			while(*keys1 == 0) {
				//do nothing
			}
			alt_up_character_lcd_init (char_lcd_dev);

		}
		while(*keys2 == 0) {
			/* Initialize the character display */
			alt_up_character_lcd_init (char_lcd_dev);
			/* Write "Welcome to" in the first row */
			alt_up_character_lcd_string(char_lcd_dev, "Chris is a");
			/* Write "the DE2 board" in the second row */
			char second_row[] = "terrible human\0";
			alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
			alt_up_character_lcd_string(char_lcd_dev, second_row);

			while(*keys2 == 0) {
				//do nothing
			}
			alt_up_character_lcd_init (char_lcd_dev);
		}
		while(*keys3 == 0) {
					/* Initialize the character display */
					alt_up_character_lcd_init (char_lcd_dev);
					/* Write "Welcome to" in the first row */
					alt_up_character_lcd_string(char_lcd_dev, "Chris smells");
					/* Write "the DE2 board" in the second row */
					char second_row[] = "like farts\0";
					alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
					alt_up_character_lcd_string(char_lcd_dev, second_row);

					while(*keys3 == 0) {
						//do nothing
					}
					alt_up_character_lcd_init (char_lcd_dev);
		}
	}
}

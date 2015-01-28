#include "altera_up_avalon_character_lcd.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#define keys1 (volatile char *) 0x0002070
#define keys2 (volatile char *) 0x0002060
#define keys3 (volatile char *) 0x0002050


int main(void) {
	// Initialize
	printf("initializing\n");
	while(1) {
		alt_up_char_buffer_dev *char_buffer;
		char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
		alt_up_char_buffer_init(char_buffer);
		// Write some text
		alt_up_char_buffer_string(char_buffer, 'E', 40, 30);
	}
}

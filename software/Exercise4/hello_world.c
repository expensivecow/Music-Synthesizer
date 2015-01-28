#include "altera_up_avalon_character_lcd.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#define keys1 (volatile char *) 0x0002070
#define keys2 (volatile char *) 0x0002060
#define keys3 (volatile char *) 0x0002050


int main(void) {

	alt_up_char_buffer_dev *char_buffer;
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	alt_up_char_buffer_init(char_buffer);
	// Use the name of your pixel buffer DMA core

	// Set the background buffer address � Although we don�t use the
	//background, they only provide a function to change the background
	//buffer address, so
	// we must set that, and then swap it to the foreground.
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
	PIXEL_BUFFER_BASE);

	// Swap background and foreground buffers
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

	// Wait for the swap to complete
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	// Clear the screen
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	// Draw a white line to the foreground buffer
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 0, 320, 240, 0x0FF0,
	0);

	// Write some text
	alt_up_char_buffer_string(char_buffer, "Lissa Smells Like Poop", 40, 30);
}

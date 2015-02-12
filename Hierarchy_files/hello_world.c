#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <time.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_keyboard.h"

#define drawer_base (volatile int *) 0x2100
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x000F
#define RED 0xF000
#define GREEN 0x0FF0
#define C1 28 // Lower C
#define Cs 29 // C#
#define D1 27 // D
#define Ds 36 // D#
#define E1 35 // E
#define F1 43 // F
#define Fs 44 // F#
#define G1 52 // G
#define Gs 53 // G#
#define A1 51 // A
#define As 60 // A#
#define B1 59 // B
#define C2 66 // Higher C
#define maxNotes 13

int main() {
	/********************************/
	/* KEYBOARD CONFIGURATIONS*******/
	/********************************/
	alt_up_ps2_dev * ps2 = alt_up_ps2_open_dev("/dev/keyboard");
	alt_up_ps2_init(ps2);
	KB_CODE_TYPE code_type;
	unsigned char buf;
	char ascii;

	/***************************************/
	/* CHAR AND BUFFER CONFIGURATIONS*******/
	/***************************************/
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev *char_buffer;

	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);

	if (pixel_buffer == 0) {
		printf(
				"error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}

	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (512 * 240 * 2);

	alt_up_char_buffer_clear(char_buffer);

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, PIXEL_BUFFER_BASE);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);

	//clear pixel buffers:
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0); //current
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1); //back buffer

	while (1) {
		//start_keyboard(ps2, code_type, buf, ascii, pixel_buffer);
		printf("Went through the start screen\n");
		int select = start_screen(pixel_buffer, char_buffer, ps2, code_type, buf, ascii);

		if(select == 1) {
			start_keyboard(ps2, code_type, buf, ascii, pixel_buffer);
		}

		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		while(alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	}
	return 0;
}

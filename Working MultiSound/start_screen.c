#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <time.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_keyboard.h"

#define drawer_base (volatile int *) 0x2100
#define SELECT_START 1
#define SELECT_USER_MANUAL 2
#define SELECT_SETTINGS 3
#define BLACK 0x0000
#define RED 0xF000

int start_screen(alt_up_pixel_buffer_dma_dev* pixel_buffer,
		alt_up_char_buffer_dev *char_buffer, alt_up_ps2_dev * ps2, KB_CODE_TYPE code_type,
		unsigned char buf, char ascii) {
	alt_up_char_buffer_clear(char_buffer);
	int select = 1; //was 1
	while (1) {
		//generate random colour:
		int colour = 0;
		int mask_iterations = 4;
		int iteration_number = 0;
		int check = decode_scancode(ps2, &code_type, &buf, &ascii);

		while (iteration_number < mask_iterations) {
			//with reference to http://stackoverflow.com/questions/7622887/generating-a-random-32-bit-hexadecimal-value-in-c
			colour |= (rand() & 0xff) << iteration_number * 8;
			iteration_number++;
		}

		alt_up_char_buffer_string(char_buffer, "Music", 38, 15);
		alt_up_char_buffer_string(char_buffer, "Synthesizer", 35, 17);

		alt_up_char_buffer_string(char_buffer, "Start", 38, 30);
		alt_up_char_buffer_string(char_buffer, "Tutorial", 36, 33);
		alt_up_char_buffer_string(char_buffer, "Settings", 36, 36);

		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

		check = decode_scancode(ps2, &code_type, &buf, &ascii);

		if(select == 1) {
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 135, 117, 185, 125,
							RED, 1);
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 135, 129, 185, 137,
							BLACK, 1);
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 135, 141, 185, 149,
							BLACK, 1);
		} else if(select == 2) {
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 135, 129, 185, 137,
							RED, 1);
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 135, 117, 185, 125,
							BLACK, 1);
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 135, 141, 185, 149,
							BLACK, 1);
		} else if(select == 3) {
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 135, 141, 185, 149,
							RED, 1);
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 135, 117, 185, 125,
							BLACK, 1);
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 135, 129, 185, 137,
							BLACK, 1);
		}

		if(code_type == 5 && buf == 114) {
			if(select == 3) {
				select = 1;
			} else {
				select++;
			}
		} else if(code_type == 5 && buf == 117) {
			if(select == 1) {
				select = 3;
			} else {
				select--;
			}
		} else if(code_type == 4 && buf == 90) {
			alt_up_char_buffer_clear(char_buffer);
			return select;
		}
		//printf("%d\n", select);


			//select = select % 3;


	}
	return 0;
}

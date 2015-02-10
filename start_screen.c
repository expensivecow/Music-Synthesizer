#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <time.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

#define drawer_base (volatile int *) 0x2100

int main()
{
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	alt_up_char_buffer_dev *char_buffer;

	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);

	if (pixel_buffer == 0) {
		printf("error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}

	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (512 * 240 * 2);

	int pixel_buffer_flag = 0;

	//with reference to http://stackoverflow.com/questions/4926622/how-to-generate-different-random-number-in-a-loop-in-c
	srand(time(0));

	alt_up_char_buffer_clear(char_buffer);
	while (1) {
		if(pixel_buffer_flag == 0) {
			alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0); //clear current screen (buffer1)
			alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);
			pixel_buffer_flag = 1; //use buffer2 next time

		}
		else {
			alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1); //clear back buffer (buffer2)
			alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);
			pixel_buffer_flag = 0; //use buffer1 next time
		}

		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

		//generate random colour:
		int colour = 0;
		int mask_iterations = 4;
		int iteration_number = 0;

		while(iteration_number < mask_iterations) {
			//with reference to http://stackoverflow.com/questions/7622887/generating-a-random-32-bit-hexadecimal-value-in-c
			colour |= (rand() & 0xff) << iteration_number*8;
			iteration_number++;
		}

		alt_up_char_buffer_string(char_buffer, "Music", 38, 15);
		alt_up_char_buffer_string(char_buffer, "Synthesizer", 35, 17);

		alt_up_char_buffer_string(char_buffer, "Start", 38, 30);
		alt_up_char_buffer_string(char_buffer, "User Manual", 35 ,33);
		alt_up_char_buffer_string(char_buffer, "Settings", 36, 36);

		IOWR_32DIRECT(drawer_base,0,135); // Set x1
		IOWR_32DIRECT(drawer_base,4,85); // Set y1
		IOWR_32DIRECT(drawer_base,8,185); // Set x2
		IOWR_32DIRECT(drawer_base,12,93); // Set y2
//		IOWR_32DIRECT(drawer_base,12,130); // Set y2
		IOWR_32DIRECT(drawer_base,16,colour);  // Set colour
//		IOWR_32DIRECT(drawer_base,16,0xAAAA);  // Set colour
		IOWR_32DIRECT(drawer_base,20,1);  // Start drawing
		while(IORD_32DIRECT(drawer_base,20)==0) {
		   //do nothing
		}; // wait until done
	}
    return 0;
}

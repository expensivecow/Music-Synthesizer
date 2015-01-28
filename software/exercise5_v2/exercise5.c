#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <time.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#define drawer_base (volatile int *) 0x2100

int main()
{
	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	if (pixel_buffer == 0) {
		printf("error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, PIXEL_BUFFER_BASE);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	/*int hw = 0;
	//with reference to http://stackoverflow.com/questions/4926622/how-to-generate-different-random-number-in-a-loop-in-c
	srand(time(0));
	while (1) {
		//with reference to http://stackoverflow.com/questions/4413170/c-generating-a-truly-random-number-between-10-20:

		//generate random coordinates (x2 > x1, y2 > y1):
		int x1 = rand() % 320;
		int y1 = rand() % 240;
		int x2 = rand() % (320 - x1) + x1;
		int y2 = rand() % (240 - y1) + y1;

		//generate random colour:
		int colour = 0;
		int mask_iterations = 4;
		int iteration_number = 0;

		while(iteration_number < mask_iterations) {
			//with reference to http://stackoverflow.com/questions/7622887/generating-a-random-32-bit-hexadecimal-value-in-c
			colour |= (rand() & 0xff) << iteration_number*8;
			iteration_number++;
		}

		printf("x1: %d, y1: %d, x2: %d, y2: %d, colour: %x\n", x1, y1, x2, y2, colour); //testing

		if (hw) {
			   IOWR_32DIRECT(drawer_base,0,x1); // Set x1
			   IOWR_32DIRECT(drawer_base,4,y2); // Set y1
			   IOWR_32DIRECT(drawer_base,8,x2); // Set x2
			   IOWR_32DIRECT(drawer_base,12,y2); // Set y2
			   IOWR_32DIRECT(drawer_base,16,colour);  // Set colour
			   IOWR_32DIRECT(drawer_base,20,1);  // Start drawing
			   while(IORD_32DIRECT(drawer_base,20)==0) {
				   //do nothing
			   }; // wait until done
		} else {
			   alt_up_pixel_buffer_dma_draw_box(pixel_buffer, x1,y1,x2,y2,colour,0);
		}
	}*/

	int hw = 0;
		if (hw) {
	           IOWR_32DIRECT(drawer_base,0,10); // Set x1
	           IOWR_32DIRECT(drawer_base,4,20); // Set y1
	           IOWR_32DIRECT(drawer_base,8,50); // Set x2
	           IOWR_32DIRECT(drawer_base,12,60); // Set y2
	           IOWR_32DIRECT(drawer_base,16,0xFFFF);  // Set colour
	           IOWR_32DIRECT(drawer_base,20,1);  // Start drawing
	           while(IORD_32DIRECT(drawer_base,20)==0); // wait until done
	 	} else {
	           alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 10,20,50,60,0xFFFF,0);
	 	}

    return 0;
}

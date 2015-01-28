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

void rectangleDraw(int, int, int, int, int);
void initializePiano(alt_up_pixel_buffer_dma_dev*);
void drawNoteLines(alt_up_pixel_buffer_dma_dev*, int);
void drawBlackKeys(alt_up_pixel_buffer_dma_dev*, int);

int main() {
	alt_up_ps2_dev  *  ps2  =  alt_up_ps2_open_dev("/dev/keyboard");
	alt_up_ps2_init(ps2);

	KB_CODE_TYPE  code_type;
	unsigned  char  buf;
	char  ascii;
	int check = decode_scancode(ps2,  &code_type,  &buf,  &ascii);
	while(1) {
		check = decode_scancode(ps2,  &code_type,  &buf,  &ascii);
		if (check >= 0) {
			printf("%d\n", ascii);
		}
	}

	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	if (pixel_buffer == 0) {
		printf("error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, PIXEL_BUFFER_BASE);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	initializePiano(pixel_buffer);

	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 40, 0, 40, 240, 0x0000, 0); // draw down lines
    return 0;
}

void initializePiano(alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	int i = 0;
	while(i < 9) {
		rectangleDraw(0,0,40*i,240,WHITE); // draw rectangular keys
		i++;
	}
	drawNoteLines(pixel_buffer, BLACK);
	drawBlackKeys(pixel_buffer, BLACK);
}

void drawBlackKeys(alt_up_pixel_buffer_dma_dev* pixel_buffer, int colour) {
	int i = 0;
	while(i < 6) {
		if(i != 2) {
			rectangleDraw(30+(40*i), 0, 50+(40*i), 120, colour);
		}
		i++;
	}
}

void drawNoteLines(alt_up_pixel_buffer_dma_dev* pixel_buffer, int colour) {
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 40, 0, 40, 240, colour, 0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 80, 0, 80, 240, colour, 0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 120, 0, 120, 240, colour, 0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 160, 0, 160, 240, colour, 0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 200, 0, 200, 240, colour, 0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 240, 0, 240, 240, colour, 0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 280, 0, 280, 240, colour, 0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 320, 0, 320, 240, colour, 0); // draw down lines
	return;
}

void rectangleDraw(int x1,int y1,int x2,int y2, int colour) {
	IOWR_32DIRECT(drawer_base,0,x1); // Set x1
	IOWR_32DIRECT(drawer_base,4,y1); // Set y1
	IOWR_32DIRECT(drawer_base,8,x2); // Set x2
	IOWR_32DIRECT(drawer_base,12,y2); // Set y2
	IOWR_32DIRECT(drawer_base,16,colour);  // Set colour
	IOWR_32DIRECT(drawer_base,20,1);  // Start drawing
	while(IORD_32DIRECT(drawer_base,20)==0); // wait until done
}

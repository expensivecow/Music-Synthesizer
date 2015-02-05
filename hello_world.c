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
#define C1 65 // Lower C
#define Cs 87 // C#
#define D1 83 // D
#define Ds 69 // D#
#define E1 68 // E
#define F1 70 // F
#define Fs 84 // F#
#define G1 71 // G
#define Gs 89 // G#
#define A1 72 // A
#define As 85 // A#
#define B1 74 // B
#define C2 75 // Higher C


void rectangleDraw(int, int, int, int, int);
void initializePiano(alt_up_pixel_buffer_dma_dev*);
void drawNoteLines(alt_up_pixel_buffer_dma_dev*, int);
void drawBlackKeys(alt_up_pixel_buffer_dma_dev*, int);
void reDrawPrevious(int);

int main() {
	alt_up_ps2_dev  *  ps2  =  alt_up_ps2_open_dev("/dev/keyboard");
	alt_up_ps2_init(ps2);
	load_audio();

	KB_CODE_TYPE  code_type;
	unsigned  char  buf;
	char  ascii;
	int currentNote = 0;

	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");

	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (512 * 240 * 2);

	if (pixel_buffer == 0) {
		printf("error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	initializePiano(pixel_buffer);

	int check = decode_scancode(ps2,  &code_type,  &buf,  &ascii);
	while(1) {
		check = decode_scancode(ps2,  &code_type,  &buf,  &ascii);
		if(check >= 0) {
			printf("%d\n", ascii);
			while(ascii > 0) {
				check = decode_scancode(ps2, &code_type, &buf, &ascii);
				if(ascii != currentNote && ascii != 0) {
					//printf("found\n");
					reDrawPrevious(currentNote);
				}
				if(ascii == C1) {
					rectangleDraw(0,0,30,120, BLUE);
					rectangleDraw(0,121,39,240, BLUE);
					currentNote = C1;
					play_sound(C1);
				} else if(ascii == Cs) {
					rectangleDraw(30,0,50,120,RED);
					currentNote = Cs;
					play_sound(Cs);
				} else if(ascii == D1) {
					rectangleDraw(51,0,69,120, GREEN);
					rectangleDraw(41,121,79,240, GREEN);
					currentNote = D1;
					play_sound(D1);
				} else if(ascii == Ds) {
					rectangleDraw(70,0,90,120,RED);
					currentNote = Ds;
					play_sound(Ds);
				} else if(ascii == E1) {
					rectangleDraw(91,0,119,120, RED);
					rectangleDraw(81,121,119,240, RED);
					currentNote = E1;
					play_sound(E1);
				} else if(ascii == F1) {
					rectangleDraw(121,0,149,120,BLUE);
					rectangleDraw(121,121,159,240,BLUE);
					currentNote = F1;
					play_sound(F1);
				} else if(ascii == Fs) {
					rectangleDraw(150,0,170,120,GREEN);
					currentNote = Fs;
					play_sound(Fs);
				} else if(ascii == G1) {
					rectangleDraw(171,0,189,120,RED);
					rectangleDraw(161,121,199,240,RED);
					currentNote = G1;
					play_sound(G1);
				} else if(ascii == Gs) {
					rectangleDraw(190,0,210,120,RED);
					currentNote = Gs;
					play_sound(Gs);
				} else if(ascii == A1) {
					rectangleDraw(211,0,229,120,BLUE);
					rectangleDraw(201,121,239,240,BLUE);
					currentNote = A1;
					play_sound(A1);
				} else if(ascii == As) {
					rectangleDraw(230,0,250,120,GREEN);
					currentNote = As;
					play_sound(As);
				} else if(ascii == B1) {
					rectangleDraw(251,0,279,120,RED);
					rectangleDraw(241,121,279,240,RED);
					currentNote = B1;
					play_sound(B1);
				} else if(ascii == C2) {
					rectangleDraw(281,0,320,240,BLUE);
					currentNote = C2;
					play_sound(C2);
				}
				else if(ascii == 0) {
					reDrawPrevious(currentNote);
					play_sound(0);
				}
			}
		}
	}
    return 0;
}

void reDrawPrevious(int ascii) {
	if(ascii == C1) {
		rectangleDraw(0,0,30,120, WHITE);
		rectangleDraw(0,121,39,239, WHITE);
	} else if(ascii == Cs) {
		rectangleDraw(30,0,50,120,BLACK);
	} else if(ascii == D1) {
		rectangleDraw(51,0,69,120, WHITE);
		rectangleDraw(41,121,79,240, WHITE);
	} else if(ascii == Ds) {
		rectangleDraw(70,0,90,120,BLACK);
	} else if(ascii == E1) {
		rectangleDraw(91,0,119,120, WHITE);
		rectangleDraw(81,121,119,240, WHITE);
	} else if(ascii == F1) {
		rectangleDraw(121,0,149,120,WHITE);
		rectangleDraw(121,121,159,240,WHITE);
	} else if(ascii == Fs) {
		rectangleDraw(150,0,170,120,BLACK);
	} else if(ascii == G1) {
		rectangleDraw(171,0,189,120,WHITE);
		rectangleDraw(161,121,199,240,WHITE);
	} else if(ascii == Gs) {
		rectangleDraw(190,0,210,120,BLACK);
	} else if(ascii == A1) {
		rectangleDraw(211,0,229,120,WHITE);
		rectangleDraw(201,121,239,240,WHITE);
	} else if(ascii == As) {
		rectangleDraw(230,0,250,120,BLACK);
	} else if(ascii == B1) {
		rectangleDraw(251,0,279,120,WHITE);
		rectangleDraw(241,121,279,240,WHITE);
	} else if(ascii == C2) {
		rectangleDraw(281,0,320,240,WHITE);
	} else {
		//no nothing
	}
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

#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <time.h>
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_keyboard.h"

#define key3Switch (volatile char *) 0x00002050
#define key2Switch (volatile char *) 0x00002060

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
void rectangleDraw(int, int, int, int, int);
void initializePiano(alt_up_pixel_buffer_dma_dev*);
void drawNoteLines(alt_up_pixel_buffer_dma_dev*, int);
void drawBlackKeys(alt_up_pixel_buffer_dma_dev*, int);
void reDrawKey(int, int);
int isValidKey(int);
int isSharp(int);
void addToPlayArray(int);

int playList[13] = {0};

int main() {
	alt_up_ps2_dev * ps2 = alt_up_ps2_open_dev("/dev/keyboard");
	alt_up_ps2_init(ps2);
	load_audio();
	int key3_reader;
	int key2_reader;

	KB_CODE_TYPE code_type;
	unsigned char buf;
	char ascii;
	int currentNote = 0;

	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	if (pixel_buffer == 0) {
		printf(
				"error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			PIXEL_BUFFER_BASE);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	initializePiano(pixel_buffer);

	int check = decode_scancode(ps2, &code_type, &buf, &ascii);

	//Keyboard loop
	while (1) {
		check = decode_scancode(ps2, &code_type, &buf, &ascii);

		key3_reader = *key3Switch;
		key2_reader = *key2Switch;

		if (isValidKey(buf)) {
			if(code_type == 1) {
				printf("code type: %d | buf: %d\n", code_type, buf);
				reDrawKey(buf, BLUE);
				addToPlayArray(buf);
			} else if(code_type == 4 && isSharp(buf)) {
				printf("code type: %d | buf: %d\n", code_type, buf);
				reDrawKey(buf, BLACK);
				removeFromPlayArray(buf);
			} else if(code_type == 4 && !isSharp(buf)) {
				printf("code type: %d | buf: %d\n", code_type, buf);
				reDrawKey(buf, WHITE);
				removeFromPlayArray(buf);
			}
			play_sound(playList,13);
		}
		if(key3_reader == 0){
			while(key3_reader == 0){
				printf("Waiting \n");
				key3_reader = *key3Switch;
			}
			printf("Key3 Switches : %i \n", key3_reader);
			changeVolume(0);
		}
		else if(key2_reader == 0){
			while(key2_reader == 0){
				printf("Waiting \n");
				key2_reader = *key2Switch;
			}
			printf("Key2 Switches : %i \n",key2_reader);
			changeVolume(1);
		}
		else{
			//printf("Key3 Switches : %i \n",key3_reader);
			//printf("Key2 Switches : %i \n",key2_reader);
		}
	}
	return 0;
}

void removeFromPlayArray(int buf) {
	int i = 0;
	while(i < maxNotes) {
		if(playList[i] == buf) {
			playList[i] = 0;
			printf("Successfully removed %d from array\n", buf);
			return;
		}
		i++;
	}
}

void addToPlayArray(int buf) {
	if(isValidKey(buf)) {
		int i = 0;
		//iterate to check if the note is already being played
		while(i < maxNotes) {
			if(playList[i] == buf) {
				return;
			}
			i++;
		}
		i = 0;
		//look for empty spot in array
		/*while(i < maxNotes) {
			if(playList[i] == 0) {
				playList[i] = buf;
				printf("adding %d to play list\n", buf);
				return;
			}
			i++;
		}*/
		if(buf==C1){
			playList[0] = buf;
			return;
		}
		else if(buf==D1){
			playList[1] = buf;
			return;
		}

		else if(buf==E1){
			playList[2] = buf;
			return;
		}
		else if(buf==F1){
			playList[3] = buf;
			return;
		}
		else if(buf==G1){
			playList[4] = buf;
			return;
		}
		else if(buf==A1){
			playList[5] = buf;
			return;
		}
		else if(buf==B1){
			playList[6] = buf;
			return;
		}
	}
}

int isSharp(int buf) {
	return (buf == Cs || buf == Ds || buf == Fs || buf == Gs || buf == As);
}

int isValidKey(int buf) {
	int * validascii[] = { C1, Cs, D1, Ds, E1, F1, Fs, G1, Gs, A1, As, B1, C2 };
	int i = 0;
	while (i < 13) {
		if (buf == validascii[i]) {
			return 1;
		}
		i++;
	}
	return 0;
}

void reDrawKey(int buf, int colour) {
	//ascii == C1
	if (buf == C1) {
		rectangleDraw(0, 0, 30, 120, colour);
		rectangleDraw(0, 121, 39, 239, colour);
	}
	//ascii == Cs
	else if (buf == Cs) {
		rectangleDraw(30, 0, 50, 120, colour);
	}
	//ascii == D1
	else if (buf == D1) {
		rectangleDraw(51, 0, 69, 120, colour);
		rectangleDraw(41, 121, 79, 240, colour);
	}
	//ascii == Ds
	else if (buf == Ds) {
		rectangleDraw(70, 0, 90, 120, colour);
	}
	//ascii == E1
	else if (buf == E1) {
		rectangleDraw(91, 0, 119, 120, colour);
		rectangleDraw(81, 121, 119, 240, colour);
	}
	//ascii == F1
	else if (buf == F1) {
		rectangleDraw(121, 0, 149, 120, colour);
		rectangleDraw(121, 121, 159, 240, colour);
	}
	//ascii == Fs
	else if (buf == Fs) {
		rectangleDraw(150, 0, 170, 120, colour);
	}
	//ascii == G1
	else if (buf == G1) {
		rectangleDraw(171, 0, 189, 120, colour);
		rectangleDraw(161, 121, 199, 240, colour);
	}
	//ascii == Gs
	else if (buf == Gs) {
		rectangleDraw(190, 0, 210, 120, colour);
	}
	//ascii == A1
	else if (buf == A1) {
		rectangleDraw(211, 0, 229, 120, colour);
		rectangleDraw(201, 121, 239, 240, colour);
	}
	//ascii == As
	else if (buf == As) {
		rectangleDraw(230, 0, 250, 120, colour);
	}
	//ascii == B1
	else if (buf == B1) {
		rectangleDraw(251, 0, 279, 120, colour);
		rectangleDraw(241, 121, 279, 240, colour);
	}
	//ascii == C2
	else if (buf == C2) {
		rectangleDraw(281, 0, 320, 240, colour);
	}
}

void initializePiano(alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	int i = 0;
	// draw 8 white rectangular keys
	while (i < 9) {
		rectangleDraw(0, 0, 40 * i, 240, WHITE);
		i++;
	}
	// draw lines between keys
	drawNoteLines(pixel_buffer, BLACK);
	// draw black keys
	drawBlackKeys(pixel_buffer, BLACK);
}

void drawBlackKeys(alt_up_pixel_buffer_dma_dev* pixel_buffer, int colour) {
	int i = 0;
	while (i < 6) {
		if (i != 2) {
			rectangleDraw(30 + (40 * i), 0, 50 + (40 * i), 120, colour);
		}
		i++;
	}
}

//quick macro for drawing note lines initially
void drawNoteLines(alt_up_pixel_buffer_dma_dev* pixel_buffer, int colour) {
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 40, 0, 40, 240, colour, 0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 80, 0, 80, 240, colour, 0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 120, 0, 120, 240, colour,
			0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 160, 0, 160, 240, colour,
			0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 200, 0, 200, 240, colour,
			0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 240, 0, 240, 240, colour,
			0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 280, 0, 280, 240, colour,
			0); // draw down lines
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 320, 0, 320, 240, colour,
			0); // draw down lines
	return;
}

//quick macro to draw rectangles using IOWR
void rectangleDraw(int x1, int y1, int x2, int y2, int colour) {
	IOWR_32DIRECT(drawer_base, 0, x1);
	// Set x1
	IOWR_32DIRECT(drawer_base, 4, y1);
	// Set y1
	IOWR_32DIRECT(drawer_base, 8, x2);
	// Set x2
	IOWR_32DIRECT(drawer_base, 12, y2);
	// Set y2
	IOWR_32DIRECT(drawer_base, 16, colour);
	// Set colour
	IOWR_32DIRECT(drawer_base, 20, 1);
	// Start drawing
	while (IORD_32DIRECT(drawer_base,20) == 0)
		; // wait until done
}

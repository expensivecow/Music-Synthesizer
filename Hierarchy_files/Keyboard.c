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
#define C1buf 28 // Lower C
#define Csbuf 29 // C#
#define D1buf 27 // D
#define Dsbuf 36 // D#
#define E1buf 35 // E
#define F1buf 43 // F
#define Fsbuf 44 // F#
#define G1buf 52 // G
#define Gsbuf 53 // G#
#define A1buf 51 // A
#define Asbuf 60 // A#
#define B1buf 59 // B
#define C2buf 66 // Higher C


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

#define maxNotes 13

int playList[13] = { 0 };
int recordStatus = 0;
int recordIndex = 0;
int keyChanged = 0;

void start_keyboard(alt_up_ps2_dev * ps2, KB_CODE_TYPE code_type,
		unsigned char buf, char ascii,
		alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	initializePiano(pixel_buffer);

	int check = decode_scancode(ps2, &code_type, &buf, &ascii);

	//Keyboard loop
	while (1) {

		int colour = 0;
		int mask_iterations = 4;
		int iteration_number = 0;

		while (iteration_number < mask_iterations) {
			//with reference to http://stackoverflow.com/questions/7622887/generating-a-random-32-bit-hexadecimal-value-in-c
			colour |= (rand() & 0xff) << iteration_number * 8;
			iteration_number++;
		}

		check = decode_scancode(ps2, &code_type, &buf, &ascii);

		if(code_type == 2 && buf == 4) {
			printf("Start playing\n");
			openSD();
			int playback[100][13];
			readTxtFile("STUPID.TXT",100,13,playback);
		}

		if (recordStatus==0 && code_type == 2 && buf == 5){
			recordStatus = 1;
			printf("Start Recording\n");
		}
		else if (recordStatus==1 && code_type == 2 && buf == 6){
			recordFinish(recordIndex);
			recordStatus = 0;
			recordIndex = 0;
			printf("End Recording\n");
		}
		if (isValidKey(buf)) {
			if (code_type == 1) {
				reDrawKey(pixel_buffer, buf, colour);
				printf("Before add %c, %d\n", ascii, ascii);
				addToPlayArray(ascii);
				keyChanged = 1;
			} else if (code_type == 4 && isSharp(buf)) {
				reDrawKey(pixel_buffer, buf, BLACK);
				removeFromPlayArray(buf);
				keyChanged = 1;
			} else if (code_type == 4 && !isSharp(buf)) {
				reDrawKey(pixel_buffer, buf, WHITE);
				removeFromPlayArray(buf);
				keyChanged = 1;
			}
			if(recordStatus == 1 && keyChanged == 1){
				printf("Record new note. \n");
				recordNote(playList,recordIndex);
				recordIndex++;
				keyChanged = 0;
			}
			play_sound(playList,13);
		}
	}
}
int returnPlayList(int size){
	return playList;
}

int convert(int buf)
{
	if(buf==C1buf){
		return C1;
	}
	else if(buf==D1buf){
		return D1;
	}
	else if(buf==E1buf){
		return E1;
	}
	else if(buf==F1buf){
		return F1;
	}
	else if(buf==G1buf){
		return G1;
	}
	else if(buf==A1buf){
		return A1;
	}
	else if(buf==B1buf){
		return B1;
	}
}

void removeFromPlayArray(int buf) {
	int ascii = convert(buf);
	int i = 0;
	while (i < maxNotes) {
		if (playList[i] == ascii) {
			playList[i] = 0;
			printf("Successfully removed %d from array\n", ascii);
			return;
		}
		i++;
	}
}

void addToPlayArray(int buf) {
	if(buf==C1){
		playList[0]=buf;
		return;
	}
	else if(buf==D1){
		playList[1]=buf;
		return;
	}
	else if(buf==E1){
		playList[2]=buf;
		return;
	}
	else if(buf==F1){
		playList[3]=buf;
		return;
	}
	else if(buf==G1){
		playList[4]=buf;
		return;
	}
	else if(buf==A1){
		playList[5]=buf;
		return;
	}
	else if(buf==B1){
		playList[6]=buf;
		return;
	}
}

int isSharp(int buf) {
	return (buf == Csbuf || buf == Dsbuf || buf == Fsbuf || buf == Gsbuf || buf == Asbuf);
}

int isValidKey(int buf) {
	int * validascii[] = { C1buf, Csbuf, D1buf, Dsbuf, E1buf, F1buf, Fsbuf, G1buf, Gsbuf, A1buf, Asbuf, B1buf, C2buf };
	int i = 0;
	while (i < 13) {
		if (buf == validascii[i]) {
			return 1;
		}
		i++;
	}
	return 0;
}

void reDrawKey(alt_up_pixel_buffer_dma_dev* pixel_buffer, int buf, int colour) {
	//ascii == C1buf
	if (buf == C1buf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 20, 30, 228, colour,
				0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 133, 39, 228, colour,
				0);
	}
	//ascii == Csbuf
	else if (buf == Csbuf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 30, 20, 50, 132, colour,
				0);
	}
	//ascii == D1buf
	else if (buf == D1buf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 51, 20, 69, 132, colour,
				0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 41, 133, 79, 228, colour,
				0);
	}
	//ascii == Dsbuf
	else if (buf == Dsbuf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 70, 20, 90, 132, colour,
				0);
	}
	//ascii == E1buf
	else if (buf == E1buf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 91, 20, 119, 132, colour,
				0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 81, 133, 119, 228,
				colour, 0);
	}
	//ascii == F1buf
	else if (buf == F1buf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 121, 20, 149, 132,
				colour, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 121, 133, 159, 228,
				colour, 0);
	}
	//ascii == Fsbuf
	else if (buf == Fsbuf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 150, 20, 170, 132,
				colour, 0);
	}
	//ascii == G1buf
	else if (buf == G1buf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 171, 20, 189, 132,
				colour, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 161, 133, 199, 228,
				colour, 0);
	}
	//ascii == Gs
	else if (buf == Gsbuf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 190, 20, 210, 132,
				colour, 0);
	}
	//ascii == A1buf
	else if (buf == A1buf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 211, 20, 229, 132,
				colour, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 201, 133, 239, 228,
				colour, 0);
	}
	//ascii == Asbuf
	else if (buf == Asbuf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 230, 20, 250, 132,
				colour, 0);
	}
	//ascii == B1buf
	else if (buf == B1buf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 251, 20, 279, 132,
				colour, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 241, 133, 279, 228,
				colour, 0);
	}
	//ascii == C2buf
	else if (buf == C2buf) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 281, 20, 319, 132,
				colour, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 281, 133, 319, 228,
				colour, 0);
	}
}

void initializePiano(alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 20, 320, 228, WHITE, 0);
	// draw lines between keys
	drawNoteLines(pixel_buffer, BLACK);
	// draw black keys
	drawBlackKeys(pixel_buffer, BLACK);
}

void drawBlackKeys(alt_up_pixel_buffer_dma_dev* pixel_buffer, int colour) {
	int i = 0;
	while (i < 6) {
		if (i != 2) {
			alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 30 + (40 * i), 12,
					50 + (40 * i), 132, colour, 0);
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

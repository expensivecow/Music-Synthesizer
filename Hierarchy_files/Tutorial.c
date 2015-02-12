#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //for sleep, but use Windows.h for Windows

#include "io.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_keyboard.h"
#define BLUE 0x000F
#define WHITE 0xFFFF
#define BLACK 0x0000
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

void start_tutorial(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev *char_buffer, alt_up_ps2_dev * ps2, KB_CODE_TYPE code_type, unsigned char buf, char ascii) {

	int waitTime = 5000000; //ns
	int stepNumber = 1;
	initializePiano(pixel_buffer, char_buffer);
	while (1) {
		//draw the piano here


		//INSTRUCTIONS:
		//need to clear char buffers in between? (this section is not tested)
		nameWhiteKeys(char_buffer);
		switch(stepNumber){
			case 1:
				alt_up_char_buffer_string(char_buffer, "Every key displayed on the screen can be played", 16, 1);
				alt_up_char_buffer_string(char_buffer, "by a corresponding key on the keyboard.", 20, 3);
				//usleep(waitTime); //wait
				stepNumber++;
				break;
			case 2:
				alt_up_char_buffer_string(char_buffer, "The white keys correspond to the ASDF row of the keyboard", 11, 1);
				//add keyboard key labels to the piano white keys?
				alt_up_char_buffer_string(char_buffer, "To play the leftmost note C, press A.", 20, 3);
				nameWhiteKeys(char_buffer);

				//detect input for ascii key "a"
				int check = decode_scancode(ps2, &code_type, &buf, &ascii);

				while(1) {
					decode_scancode(ps2, &code_type, &buf, &ascii);
					if(code_type == 1 && buf == 28) {
						printf("code type: %d | buf: %d\n", code_type, buf);
						reDrawKey(pixel_buffer, buf, BLUE);
						addToPlayArray(ascii);
						play_sound(returnPlayList(13),13);
					}
					else if(code_type == 4 && buf == C1){
						reDrawKey(pixel_buffer, buf, WHITE);
						removeFromPlayArray(buf);
						alt_up_char_buffer_clear(char_buffer);
						nameWhiteKeys(char_buffer);
						alt_up_char_buffer_string(char_buffer, "Nice job! We will be moving on now.", 20, 1);
						break;
					}
				}
				stepNumber++;
				break;
			case 3:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The white keys correspond to the ASDF row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "To play the adjacent note D, press S.", 20, 3);

				nameWhiteKeys(char_buffer);

				while(1) {
					decode_scancode(ps2, &code_type, &buf, &ascii);
					if(code_type == 1 && buf == D1) {
						printf("code type: %d | buf: %d\n", code_type, buf);
						reDrawKey(pixel_buffer, buf, BLUE);
						addToPlayArray(ascii);
						play_sound(returnPlayList(13),13);
					}
					else if(code_type == 4 && buf == D1){
						reDrawKey(pixel_buffer, buf, WHITE);
						removeFromPlayArray(buf);
						alt_up_char_buffer_clear(char_buffer);
						nameWhiteKeys(char_buffer);
						alt_up_char_buffer_string(char_buffer, "Nice job! We will be moving on now.", 20, 1);
						break;
					}
				}

				stepNumber++;
				break;

			case 4:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The white keys correspond to the ASDF row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "To play the rightmost note C, press K.", 20, 3);


				nameWhiteKeys(char_buffer);
				while(1) {
					decode_scancode(ps2, &code_type, &buf, &ascii);
					if(code_type == 1 && buf == C2) {
						printf("code type: %d | buf: %d\n", code_type, buf);
						reDrawKey(pixel_buffer, buf, BLUE);
						addToPlayArray(buf);
						play_sound(returnPlayList(13),13);
					}
					else if(code_type == 4 && buf == C2){
						reDrawKey(pixel_buffer, buf, WHITE);
						removeFromPlayArray(buf);
						alt_up_char_buffer_clear(char_buffer);
						nameWhiteKeys(char_buffer);
						alt_up_char_buffer_string(char_buffer, "Nice job! We will be moving on now.", 20, 1);
						break;
					}
				}

				stepNumber++;
				break;
			case 5:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				//add keyboard key labels to the piano black keys?
				alt_up_char_buffer_string(char_buffer, "To play the leftmost note C# (or Db), press W.", 16, 3);

				nameWhiteKeys(char_buffer);

				while(1) {
					decode_scancode(ps2, &code_type, &buf, &ascii);
					if(code_type == 1 && buf == Cs) {
						printf("code type: %d | buf: %d\n", code_type, buf);
						reDrawKey(pixel_buffer, buf, BLUE);
						addToPlayArray(ascii);
						play_sound(returnPlayList(13),13);
					}
					else if(code_type == 4 && buf == Cs){
						reDrawKey(pixel_buffer, buf, BLACK);
						removeFromPlayArray(buf);
						alt_up_char_buffer_clear(char_buffer);
						nameWhiteKeys(char_buffer);
						alt_up_char_buffer_string(char_buffer, "Nice job! We will be moving on now.", 20, 1);
						break;
					}
				}

				stepNumber++;
				break;

			case 6:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "To play the adjacent note D# (or Eb), press E.", 16, 3);

				nameWhiteKeys(char_buffer);


				while(1) {
					decode_scancode(ps2, &code_type, &buf, &ascii);
					if(code_type == 1 && buf == Ds) {
						printf("code type: %d | buf: %d\n", code_type, buf);
						reDrawKey(pixel_buffer, buf, BLUE);
						addToPlayArray(ascii);
						play_sound(returnPlayList(13),13);
					}
					else if(code_type == 4 && buf == Ds){
						reDrawKey(pixel_buffer, buf, BLACK);
						removeFromPlayArray(buf);
						alt_up_char_buffer_clear(char_buffer);
						nameWhiteKeys(char_buffer);
						alt_up_char_buffer_string(char_buffer, "Nice job! We will be moving on now.", 20, 1);
						break;
					}
				}

				stepNumber++;
				break;

			case 7:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "Notice the correspondence between the keyboard and the piano on the screen.", 5, 3);
				//label the white and black keys with ascii keys here?

				nameWhiteKeys(char_buffer);
				stepNumber++;
				break;

			case 8:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "Now try pressing R on your keyboard.", 20, 3);

				stepNumber++;
				break;

			case 9:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "Notice that no note is played since there is no black key between notes E an F.", 1, 3);

				//detect input for ascii key "

				nameWhiteKeys(char_buffer);
				stepNumber++;
				break;

			case 10:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "Congratulations,  You are now ready to start!", 15, 1);
				alt_up_char_buffer_string(char_buffer, "Press ENTER to continue.", 25, 3);
				//detect input for ascii key "enter"

				nameWhiteKeys(char_buffer);
				while(1) {
					decode_scancode(ps2, &code_type, &buf, &ascii);
					if(code_type == 4 && buf == 90) {
						alt_up_char_buffer_clear(char_buffer);
						return;
					}
				}

				//stepNumber++;
				break;
		}

		usleep(waitTime); //wait //testing
		alt_up_char_buffer_clear(char_buffer);

		//alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		//while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	}
	return;
}

void nameWhiteKeys(alt_up_char_buffer_dev *char_buffer){
	int whiteKeyNum = 0;
	int maxNumWhiteKeys = 8;
	int y = 58;
	while(whiteKeyNum < maxNumWhiteKeys){
//		int x = 20 + whiteKeyNum*40;
		int x = 5 + whiteKeyNum*10;

		switch(whiteKeyNum){
			case 1:
				alt_up_char_buffer_string(char_buffer, "D", x, y);
				break;
			case 2:
				alt_up_char_buffer_string(char_buffer, "E", x, y);
				break;
			case 3:
				alt_up_char_buffer_string(char_buffer, "F", x, y);
				break;
			case 4:
				alt_up_char_buffer_string(char_buffer, "G", x, y);
				break;
			case 5:
				alt_up_char_buffer_string(char_buffer, "A", x, y);
				break;
			case 6:
				alt_up_char_buffer_string(char_buffer, "B", x, y);
				break;
			default:
				alt_up_char_buffer_string(char_buffer, "C", x, y);
				break;
		}
		whiteKeyNum++;
	}
}

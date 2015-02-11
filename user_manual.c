#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //for sleep, but use Windows.h for Windows

#include "io.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

#define drawer_base (volatile int *) 0x2100

//char getWhiteKeyName(int whiteKeyNum);

int main() {
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

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);

	//clear screens first
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	int stepNumber = 1;
	while (1) {
		//draw the piano here

		//naming the white keys only
		int whiteKeyNum = 0;
		int maxNumWhiteKeys = 8;
		while(whiteKeyNum < maxNumWhiteKeys){
			int x = 20 + whiteKeyNum*40;
//			char keyName = getWhiteKeyName(whiteKeyNum);

			switch(whiteKeyNum){
				case 1:
					alt_up_char_buffer_string(char_buffer, "D", x, 239);
					break;
				case 2:
					alt_up_char_buffer_string(char_buffer, "E", x, 239);
					break;
				case 3:
					alt_up_char_buffer_string(char_buffer, "F", x, 239);
					break;
				case 4:
					alt_up_char_buffer_string(char_buffer, "G", x, 239);
					break;
				case 5:
					alt_up_char_buffer_string(char_buffer, "A", x, 239);
					break;
				case 6:
					alt_up_char_buffer_string(char_buffer, "B", x, 239);
					break;
				default:
					alt_up_char_buffer_string(char_buffer, "C", x, 239);
					break;
			}

//			alt_up_char_buffer_string(char_buffer, whiteKeyName, x, 239);
			whiteKeyNum++;
		}

		//INSTRUCTIONS:
		//need to clear char buffers in between? (this section is not tested)
		switch(stepNumber){
			case 1:
				alt_up_char_buffer_string(char_buffer, "Every key displayed on the screen can be played", 7, 1);
				alt_up_char_buffer_string(char_buffer, "by a corresponding key on the keyboard.", 10, 3);
				sleep(1000); //wait for 1 second;
				stepNumber++;
				break;
			case 2:
				alt_up_char_buffer_string(char_buffer, "The white keys correspond to the ASDF row of the keyboard", 7, 1);
				//add keyboard key labels to the piano white keys?
				alt_up_char_buffer_string(char_buffer, "To play the leftmost note C, press A.", 10, 3);
				//detect input for asci key "a"
				sleep(1000); //wait for 1 second;

				alt_up_char_buffer_string(char_buffer, "To play the adjacent note D, press S.", 10, 3);
				//detect input for asci key "s"
				sleep(1000); //wait for 1 second;

				alt_up_char_buffer_string(char_buffer, "To play the rightmost note C, press K.", 10, 3);
				//detect input for asci key "k"
				sleep(1000); //wait for 1 second;

				stepNumber++;
				break;
			case 3:
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 7, 1);
				//add keyboard key labels to the piano black keys?
				alt_up_char_buffer_string(char_buffer, "To play the leftmost note C#(or Db), press W.", 10, 3);
				//detect input for asci key "w"
				sleep(1000); //wait for 1 second;

				alt_up_char_buffer_string(char_buffer, "To play the adjacent note D# (or Eb), press E.", 10, 3);
				//detect input for asci key "e"
				alt_up_char_buffer_string(char_buffer, "Notice the correspondence between the keyboard and the piano on the screen.", 10, 3);
				//label the white and black keys with asci keys here?
				sleep(1000); //wait for 1 second;

				alt_up_char_buffer_string(char_buffer, "Now try pressing R on your keyboard.", 10, 3);
				//detect input for asci key "r"
				alt_up_char_buffer_string(char_buffer, "Notice that no note is played since there is no black key between notes E an F.", 10, 3);
				sleep(1000); //wait for 1 second;

				stepNumber++;
				break;

			case 4:
				alt_up_char_buffer_string(char_buffer, "Congratulations,  You are now ready to start!", 10, 1);
				alt_up_char_buffer_string(char_buffer, "Press ENTER to continue.", 10, 3);
				//detect input for asci key "enter"
				//exit procede to start (no menu screen)
				stepNumber++;
				break;
		}

		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	}
	return 0;
}

//char* getWhiteKeyName(int whiteKeyNum){
//	char* whiteKeyName;
//
//	switch(whiteKeyNum){
//		case 1:
//			*whiteKeyName = "D";
//			break;
//		case 2:
//			*whiteKeyName = "E";
//			break;
//		case 3:
//			*whiteKeyName = "F";
//			break;
//		case 4:
//			*whiteKeyName = "G";
//			break;
//		case 5:
//			*whiteKeyName = "A";
//			break;
//		case 6:
//			*whiteKeyName = "B";
//			break;
//		default:
//			*whiteKeyName = "C";
//			break;
//	}
//
//	return whiteKeyName;
//}

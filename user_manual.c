#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> //for sleep, but use Windows.h for Windows

#include "io.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"

#define drawer_base (volatile int *) 0x2100

void nameWhiteKeys(alt_up_char_buffer_dev *char_buffer);
void drawPiano(alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev *char_buffer);

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

	int waitTime = 10000000; //ns
	int stepNumber = 1;
	while (1) {
		//draw the piano here


		//INSTRUCTIONS:
		//need to clear char buffers in between? (this section is not tested)

		switch(stepNumber){
			case 1:
				alt_up_char_buffer_string(char_buffer, "Every key displayed on the screen can be played", 16, 1);
				alt_up_char_buffer_string(char_buffer, "by a corresponding key on the keyboard.", 20, 3);
				drawPiano(pixel_buffer, char_buffer);
				//usleep(waitTime); //wait
				stepNumber++;
				break;
			case 2:
				alt_up_char_buffer_string(char_buffer, "The white keys correspond to the ASDF row of the keyboard", 11, 1);
				//add keyboard key labels to the piano white keys?
				alt_up_char_buffer_string(char_buffer, "To play the leftmost note C, press A.", 20, 3);
				drawPiano(pixel_buffer, char_buffer);

				//detect input for ascii key "a"

				stepNumber++;
				break;

			case 3:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The white keys correspond to the ASDF row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "To play the adjacent note D, press S.", 20, 3);
				drawPiano(pixel_buffer, char_buffer);

				//detect input for ascii key "s"

				stepNumber++;
				break;

			case 4:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The white keys correspond to the ASDF row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "To play the rightmost note C, press K.", 20, 3);
				drawPiano(pixel_buffer, char_buffer);

				//detect input for ascii key "k"

				stepNumber++;
				break;
			case 5:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				//add keyboard key labels to the piano black keys?
				alt_up_char_buffer_string(char_buffer, "To play the leftmost note C# (or Db), press W.", 16, 3);
				drawPiano(pixel_buffer, char_buffer);

				//detect input for ascii key "w"

				stepNumber++;
				break;

			case 6:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "To play the adjacent note D# (or Eb), press E.", 16, 3);
				drawPiano(pixel_buffer, char_buffer);

				//detect input for ascii key "e"

				stepNumber++;
				break;

			case 7:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "Notice the correspondence between the keyboard and the piano on the screen.", 5, 3);
				//label the white and black keys with ascii keys here?
				drawPiano(pixel_buffer, char_buffer);

				stepNumber++;
				break;

			case 8:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "Now try pressing R on your keyboard.", 20, 3);
				drawPiano(pixel_buffer, char_buffer);

				//detect input for ascii key "r"

				stepNumber++;
				break;

			case 9:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "The black keys correspond to the QWER row of the keyboard", 11, 1);
				alt_up_char_buffer_string(char_buffer, "Notice that no note is played since there is no black key between notes E an F.", 6, 3);
				drawPiano(pixel_buffer, char_buffer);

				//detect input for ascii key "

				stepNumber++;
				break;

			case 10:
				alt_up_char_buffer_clear(char_buffer);
				alt_up_char_buffer_string(char_buffer, "Congratulations,  You are now ready to start!", 215, 1);
				alt_up_char_buffer_string(char_buffer, "Press ENTER to continue.", 20, 3);
				//detect input for ascii key "enter"
				//exit proceed to start (no menu screen)

				drawPiano(pixel_buffer, char_buffer);

				//stepNumber++;
				break;
		}

		usleep(waitTime); //wait //testing
		alt_up_char_buffer_clear(char_buffer);

		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));
	}
	return 0;
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


void drawPiano(	alt_up_pixel_buffer_dma_dev* pixel_buffer, alt_up_char_buffer_dev *char_buffer){
	nameWhiteKeys(char_buffer);
}

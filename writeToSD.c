#include <stdio.h>
#include <stdlib.h>
#include "altera_up_sd_card_avalon_interface.h"
#include <sys/alt_irq.h>

short int write_file = 0;

//  \r is carriage return, \r\n is new line in windows \0 is the null value for end of string
char buffer[40] = "HELLO!\r\n\0";

void write_sd(char *desiredfile)
{
	int connected = 0;
	alt_up_sd_card_dev *device_reference = NULL;
	device_reference = alt_up_sd_card_open_dev(ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME);

	if (device_reference != NULL)
	{
		if ((connected == 0) && (alt_up_sd_card_is_Present()))
		{
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16())
				printf("FAT16 file system detected.\n");
			else
				printf("Unknown file system.\n");
			connected = 1;
			write_file = alt_up_sd_card_fopen(desiredfile, true);
			 if (write_file < 0){
				  printf("Problem creating file. Error %i", write_file);
			 }
			 else
			 {
				  printf("SD card accessed\n");
				  int index = 0;
				  //While not null, or at the pend of the file
				  while (buffer[index] != '\0')
				  {
					  alt_up_sd_card_write(write_file, buffer[index]);
					  //Increment the file
					  index = index + 1;
				  }
				  printf("Writing successful\n");

				  printf("Closing File\n");
				  alt_up_sd_card_fclose(write_file);
				  printf("%i\n",write_file);
				  printf("Done\n");
			  }
		}
		else if ((connected == 1) && (alt_up_sd_card_is_Present() == false))
		{
			printf("Card disconnected.\n");
			connected = 0;
		}
	}
}

int main (void){
	write_sd("TEST.TXT");
	return 0;
}

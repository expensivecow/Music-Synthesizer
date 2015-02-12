#include <stdio.h>
#include <stdlib.h>
#include "altera_up_sd_card_avalon_interface.h"
#include <sys/alt_irq.h>

short int write_file = 0;

//  \r is carriage return, \r\n is new line in windows \0 is the null value for end of string
int buffer[100][13];
char key;


void write_sd(char *desiredfile)
{
	int temp;

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

			if(checkDuplicate(desiredfile)){
				printf("Duplicate file found. %s \n",desiredfile);
			}

			temp=write_file = alt_up_sd_card_fopen(desiredfile, false);

			printf("%i fopen\n",temp);

			 if (write_file < 0){
				  printf("Problem creating file. Error %i", write_file);
			 }
			 else
			 {
				  printf("SD card accessed\n");
				  int index = 0;
				  //While not null, or at the pend of the file
				  while (buffer[index][0] != 4)
				  {
					  int i;
					  for(i = 0; i<13; i++)
					  {
					  alt_up_sd_card_write(write_file, buffer[index][i]);
					  }
					  alt_up_sd_card_write(write_file, '\n');
					  //Increment the file
					  index = index + 1;
				  }
				  alt_up_sd_card_write(write_file, 4);
				  printf("Writing successful\n");

				  printf("Closing File\n");
				  temp = alt_up_sd_card_fclose(write_file);
				  printf("%i\n",temp);
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

int checkDuplicate(char *check_file){
	short int file_found = 0;
	char *file_name;
	int exist = 0;
	int duplicate = 0;
	file_found = alt_up_sd_card_find_first ("//", file_name);
	while (file_found == 0)
	{
		file_found = alt_up_sd_card_find_next(file_name);
		if (file_found == 0)
		{
			exist = 1;
			printf ("%s\n", file_name);
			if(strcmp(check_file,file_name)){
				duplicate = 1;
				return 1;
			}
		}
		else if (file_found == 1)
			printf ("Invalid directory.\n");
		else if (file_found == -1 && exist == 0)
			printf ("Card empty.\n");
	}
	return 0;
}

/*void loadWriteData(char writeBuffer[]){
	int index = 0;
	while (buffer[index] != '\0'){
	buffer[index] = writeBuffer[index];
	index++;
	}
}*/

void recordNote(int keys[], int bufferIndex){
	int i;
	for(i = 0; i < 13; i ++)
	{
		buffer[bufferIndex][i] = keys[i];
	}
}

void recordFinish(int bufferIndex){
	buffer[bufferIndex][0]=4;
	write_sd("Multi");
}


/*int main(void){

	write_sd("SONG.TXT");
	return 0;
}*/

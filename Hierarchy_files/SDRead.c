
#include <stdio.h>
#include <stdlib.h>
#include "altera_up_sd_card_avalon_interface.h"

int num_of_txt = 0;
char **list_of_txt;
alt_up_sd_card_dev *device_reference = NULL;
short int songFile;

void openSD(void)
{
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	device_reference = alt_up_sd_card_open_dev(ALTERA_UP_SD_CARD_AVALON_INTERFACE_0_NAME);
	if (device_reference != NULL) {
		if ((connected == 0) && (alt_up_sd_card_is_Present())) {
			printf("Card connected.\n");
			if (alt_up_sd_card_is_FAT16()) {
				printf("FAT16 file system detected.\n");
			} else {
				printf("Unknown file system.\n");
			}
		}
	}
}

//Scans files in SD card
//Gets TXT files and puts into list_of_txt
void getTxtFiles (void)
{
	short int file_found = 0;
	char *file_name;
	int count = 0;

	//Gets number of txt files
	file_found = alt_up_sd_card_find_first ("//", file_name);
	while (file_found == 0)
	{
		file_found = alt_up_sd_card_find_next(file_name);
		if (file_found == 0)
		{
			char* extension = strstr(file_name, ".TXT");
			if(strcmp(extension, ".TXT") == 0)
			{
				num_of_txt++;
			}
		}
		else if (file_found == 1)
		{
			printf ("Invalid directory.\n");
		}
	}
	//Allocates memory for txt files
	list_of_txt = malloc(num_of_txt * sizeof(char*));
	int i;
	for(i = 0; i < num_of_txt; i++)
	{
		list_of_txt[i] = malloc(12 * sizeof(char));
	}

	//Places names of txt files into array
	file_found = alt_up_sd_card_find_first ("//", file_name);
	while (file_found == 0)
	{
		file_found = alt_up_sd_card_find_next(file_name);
		if (file_found == 0)
		{
			char* extension = strstr(file_name, ".TXT");
			if(strcmp(extension, ".TXT") == 0)
			{
				printf("%s\n",file_name);
				strcpy(list_of_txt[count],file_name);
				count++;
			}
		}
	}
}

void displayTxtFiles (void)
{
	int i;
	printf("%d\n",num_of_txt);
	for(i = 0; i < num_of_txt; i++)
	{
		printf("%s\n",list_of_txt[i]);
	}
}

//Might need a return
//Should convert contents into sound
void readTxtFile (char* filename, size_t m, size_t n, int playback[m][n],int timer[m])
{
	unsigned int letter;
	int count = 0;
	songFile = alt_up_sd_card_fopen(filename, 0);
	printf("File test %d\n", songFile);
	while(1)
	{
		int i;
		for(i = 0; i < 13; i++)
		{
			//Reads next letter
			letter = alt_up_sd_card_read(songFile);
			playback[count][i] = letter;
			//Check if EOF reacheds
			if(letter == -1)
			{
				alt_up_sd_card_fclose(songFile);
				break;
			}
		}
		letter = alt_up_sd_card_read(songFile);
		timer[count] = letter;
		if(letter == -1)
		{
			timer[count + 1] = -1;
			break;
		}
		//Skips /n char
		letter = alt_up_sd_card_read(songFile);
		count++;
	}
}

/*void main(void)
{
	openSD();
	getTxtFiles();
	displayTxtFiles();
}*/

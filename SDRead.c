
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
			printf ("%s\n", file_name);
			int length = strlen(file_name);
			char* extension = file_name[length - 4];
			if(strcmp(extension, ".txt"))
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

	//Places names of txt files into array
	file_found = alt_up_sd_card_find_first ("//", file_name);
	while (file_found == 0)
	{
		file_found = alt_up_sd_card_find_next(file_name);
		if (file_found == 0)
		{
			int length = strlen(file_name);
			char* extension = file_name[length - 4];
			if(strcmp(extension, ".txt"))
			{
				list_of_txt[count] = file_name;
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
int** readTxtFile (char* filename)
{
	int** contents;
	unsigned int letter;
	songFile = alt_up_sd_card_fopen(filename, 0);
	do{
		letter = alt_up_sd_card_read(songFile);
		printf("%d\n",letter);
	}while(letter != -1);
	
	return contents;
}

void main(void)
{
	openSD();
	getTxtFiles();
	displayTxtFiles();
}

#define switches (volatile char *) 0x00004850

#include <stdio.h>
#include <stdlib.h>
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include "altera_up_sd_card_avalon_interface.h"
#include <sys/alt_irq.h>

//General
alt_up_audio_dev *audio;
short int opened_file = 0;
int sample_size = 110; //ISR: 96 Hit: 2

//Hit
unsigned int *audio_data_16_hit;
int hit_file_size = 0;
int audio_data_16_hit_current_index = 0;

//Laser
unsigned int *audio_data_16_laser;
int laser_file_size = 0;
int audio_data_16_laser_current_index = 0;

//ISR
char *filename;
unsigned int *audio_buffer;

int sw_reader;

void read_sd (void);


void audio_isr(void* context, alt_u32 id)
{
	int audio_buffer_count;

	sw_reader = *switches;
	sw_reader = sw_reader & 0x0001;
	//printf("Switch reader %i. \n",sw_reader);

	if(sw_reader == 0x0001) //Condition for hit sound
	{
		printf("Switch up.\n");
		for(audio_buffer_count = 0 ; audio_buffer_count < sample_size; audio_buffer_count++)
		{
			audio_buffer[audio_buffer_count] = 0;

			if(audio_data_16_hit_current_index < hit_file_size/2)
			{
				audio_buffer[audio_buffer_count] += (audio_data_16_hit[audio_data_16_hit_current_index])/2;

				audio_data_16_hit_current_index++;

				/*if(audio_data_16_hit_current_index >= hit_file_size/2)
				{
					audio_data_16_hit_current_index = 0;
				}
				else
				{
					audio_data_16_hit_current_index++;
				}*/

			}
			if(audio_data_16_laser_current_index < laser_file_size/2)
			{
				audio_buffer[audio_buffer_count] += (audio_data_16_laser[audio_data_16_laser_current_index]/2);

				audio_data_16_laser_current_index++;

				/*if(audio_data_16_laser_current_index >= laser_file_size/2)
				{
					audio_data_16_laser_current_index = 0;
				}
				else
				{
					audio_data_16_laser_current_index++;
				}*/
			}
		}
	}
	else
	{
		audio_data_16_hit_current_index = 0;
		audio_data_16_laser_current_index = 0;
		for(audio_buffer_count = 0 ; audio_buffer_count < sample_size; audio_buffer_count++)
		{
			audio_buffer[audio_buffer_count]=0;
		}
	}

	alt_up_audio_write_fifo(audio, audio_buffer, sample_size, ALT_UP_AUDIO_LEFT);
	alt_up_audio_write_fifo(audio, audio_buffer, sample_size, ALT_UP_AUDIO_RIGHT);
}

short convert_to_16 (char a, char b)
{
	short c = (unsigned char) a << 8 | (unsigned char) b;

	return c;
}

void open_file(filename)
{
	opened_file = alt_up_sd_card_fopen(filename, 0);

	if (opened_file == -1)
		printf("File could not be opened\n");

	if (opened_file == -2)
		printf("File is already opened\n");

	printf("File is opened.\n");
}

void reverse_array(int array[], int count)
{
	int temp, i;

	for (i = 0; i < count/2; ++i)
	{
		temp = array[i];
		array[i] = array[count-i-1];
		array[count-i-1] = temp;
	}
}

void audio_file_info()
{
	unsigned int audio_file_header[40];
	unsigned int audio_file_size[4];

	int i;
	int j = 0;


	for(i = 0; i < 40; i++)
	{
		audio_file_header[i] = alt_up_sd_card_read(opened_file);
	}


	for(i = 4; i < 8; i++ )
	{
		audio_file_size[j] = audio_file_header[i];

		j++;
	}

	reverse_array(audio_file_size, 4);

	if(filename == "Jump5.wav")
	{
		hit_file_size = (audio_file_size[0] << 8*3 | audio_file_size[1] << 8*2 | audio_file_size[2] << 8 | audio_file_size[3]) + 8;
		printf("Jump5 size: %d\n", hit_file_size);
	}
	else{
		laser_file_size = (audio_file_size[0] << 8*3 | audio_file_size[1] << 8*2 | audio_file_size[2] << 8 | audio_file_size[3]) + 8;
		printf("Laser size: %d\n", laser_file_size);
	}

}

void audio_data_load()
{
	int i,j;
	unsigned int audio_data_byte[2];

	if(filename == "Jump5.wav")
	{
		//Allocates memory space for buffer
		audio_data_16_hit = (unsigned int *) malloc ((hit_file_size/2) * sizeof(unsigned int));

		printf("Loading Jump5 data\n");

		for(i = 0; i < (hit_file_size/2); i++)
		{
			for(j = 0; j < 2; j++)
			{
				audio_data_byte[j] = alt_up_sd_card_read(opened_file) & (0x00FF);
			}

			audio_data_16_hit[i] = convert_to_16(audio_data_byte[1], audio_data_byte[0]);
		}

		printf("Finish loading Jump5 data\n");
	}

	else
	{
		audio_data_16_laser = (unsigned int *) malloc ((laser_file_size/2) * sizeof(unsigned int));

		printf("Loading Laser data\n");

		for(i = 0; i < (laser_file_size/2); i++)
		{
			for(j = 0; j < 2; j++)
			{
				audio_data_byte[j] = alt_up_sd_card_read(opened_file) & (0x00FF);
			}

			audio_data_16_laser[i] = convert_to_16(audio_data_byte[1], audio_data_byte[0]);
		}

		printf("Finish loading Laser data\n");
	}

}

void audio_initialize()
{
	audio = alt_up_audio_open_dev(AUDIO_0_NAME);

	alt_up_audio_reset_audio_core(audio);

	audio_buffer = (unsigned int *) malloc (sample_size * sizeof(unsigned int));
}

void start_audio_interrupt()
{
	if(alt_irq_register(AUDIO_0_IRQ, NULL ,audio_isr) == 0)
		printf("irq_register success\n");
	else
		printf("irq_register fail\n");

	printf("Enable Interrupt\n");
	alt_up_audio_enable_write_interrupt(audio);
}

void stop_audio_interrupt()
{
	alt_up_audio_disable_write_interrupt(audio);
}

void load_music(char *selectedFile)
{
	filename = selectedFile;
	read_sd();
	open_file(filename);
	audio_file_info();
	audio_data_load();
}

int main(void)
{
	sw_reader = *switches;
	sw_reader = sw_reader & 0x0001;

	load_music("Jump5.wav");
	load_music("Laser.wav");

	audio_initialize();
	start_audio_interrupt();

}

void read_sd (void)
{
	int connected = 0;
	int exist = 0;
	short int file_found = 0;
	char *file_name;

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

			file_found = alt_up_sd_card_find_first ("//", file_name);

			while (file_found == 0)
			{
				file_found = alt_up_sd_card_find_next(file_name);

				if (file_found == 0)
				{
					exist = 1;
					printf ("%s\n", file_name);
				}

				else if (file_found == 1)
					printf ("Invalid directory.\n");

				else if (file_found == -1 && exist == 0)
					printf ("Card empty.\n");
			}
		}

		else if ((connected == 1) && (alt_up_sd_card_is_Present() == false))
		{
			printf("Card disconnected.\n");

			connected = 0;
		}
	}

	//printf ("Error, please check card.\n");
}
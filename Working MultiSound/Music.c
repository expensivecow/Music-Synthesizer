#define switches (volatile char *) 0x00002010
#define key3Switch (volatile char *) 0x00002050
#define key2Switch (volatile char *) 0x00002060

#include <stdio.h>
#include <stdlib.h>
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include "altera_up_sd_card_avalon_interface.h"
#include <sys/alt_irq.h>
#include "altera_up_avalon_character_lcd.h"

//Defines for piano keys
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

//General
alt_up_audio_dev *audio;
short int opened_file = 0;
int sample_size = 110;
int file_size = 0; //Universal file size for sounds
//Volume
int volume = 5;

//C
unsigned int *c_data;
int c_index = 0;

//D
unsigned int *d_data;
int d_index = 0;

//E
unsigned int *e_data;
int e_index = 0;

//F
unsigned int *f_data;
int f_index = 0;

//G
unsigned int *g_data;
int g_index = 0;

//A
unsigned int *a_data;
int a_index = 0;

//B
unsigned int *b_data;
int b_index = 0;

char *filename;
unsigned int *audio_buffer;

int* keys;
int keySize = 0;

void read_sd(void);

void volumeLCD(void)
{
	alt_up_character_lcd_dev * char_lcd_dev;
	// open the Character LCD port
	char_lcd_dev = alt_up_character_lcd_open_dev ("/dev/character_lcd_0");
	if ( char_lcd_dev == NULL)
		alt_printf ("Error: could not open character LCD device\n");
	else
		alt_printf ("Opened character LCD device\n");
	/* Initialize the character display */
	alt_up_character_lcd_init (char_lcd_dev);
	alt_up_character_lcd_string(char_lcd_dev, "Volume:");
	char ascii = (char) (volume + (int)'0');
	char second_row[] = {ascii, '\0'};
	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
	alt_up_character_lcd_string(char_lcd_dev, second_row);
}

void changeVolume(int change){

	if(change == 0){
		printf("Volume Before : %i \n",volume);
		if(volume>1 && volume<=9){
			volume--;
			printf("Volume : %i \n",volume);
		}
	}

	else if(change == 1){
		printf("Volume : %i \n",volume);
		if(volume>=1 && volume<9){
			volume++;
			printf("Volume : %i \n",volume);
		}
	}

	volumeLCD();
}

bool checkZero(void)
{
	int i;
	for(i = 0; i < keySize; i++)
	{
		if(keys[i] != 0)
		{
			return false;
		}
	}
	return true;
}

void audio_isr(void* context, alt_u32 id) {
	int audio_buffer_count;
	//double finalVolume = pow(2,(volume-7));
	//unsigned int finalVolume = 4;
	if(!checkZero())
	{
		for(audio_buffer_count = 0 ; audio_buffer_count < sample_size; audio_buffer_count++)
		{
			audio_buffer[audio_buffer_count] = 0;

			if(keys[0] == C1 && c_index < file_size/2)
			{
				audio_buffer[audio_buffer_count] += (c_data[c_index])/2;

				c_index++;

				if(c_index >= file_size/2)
				{
					c_index = 0;
				}
				else
				{
					c_index++;
				}
			}
			if(keys[1] == D1 && d_index < file_size/2)
			{
				audio_buffer[audio_buffer_count] += (d_data[d_index])/2;

				d_index++;

				if(d_index >= file_size/2)
				{
					d_index = 0;
				}
				else
				{
					d_index++;
				}
			}

			if(keys[2] == E1 && e_index < file_size/2)
			{
				audio_buffer[audio_buffer_count] += (e_data[e_index])/2;

				e_index++;

				if(e_index >= file_size/2)
				{
					e_index = 0;
				}
				else
				{
					e_index++;
				}
			}

			if(keys[3] == F1 && f_index < file_size/2)
			{
				audio_buffer[audio_buffer_count] += (f_data[f_index])/2;

				f_index++;

				if(f_index >= file_size/2)
				{
					f_index = 0;
				}
				else
				{
					f_index++;
				}
			}

			if(keys[4] == G1 && g_index < file_size/2)
			{
				audio_buffer[audio_buffer_count] += (g_data[g_index])/2;

				g_index++;

				if(g_index >= file_size/2)
				{
					g_index = 0;
				}
				else
				{
					g_index++;
				}
			}

			if(keys[5] == A1 && a_index < file_size/2)
			{
				audio_buffer[audio_buffer_count] += (a_data[a_index])/2;

				a_index++;

				if(a_index >= file_size/2)
				{
					a_index = 0;
				}
				else
				{
					a_index++;
				}
			}

			if(keys[6] == B1 && b_index < file_size/2)
			{
				audio_buffer[audio_buffer_count] += (b_data[b_index])/2;

				b_index++;

				if(b_index >= file_size/2)
				{
					b_index = 0;
				}
				else
				{
					b_index++;
				}
			}
		}
	}

	else
	{
		c_index = 0;
		d_index = 0;
		e_index = 0;
		f_index = 0;
		g_index = 0;
		a_index = 0;
		b_index = 0;
		for(audio_buffer_count = 0 ; audio_buffer_count < sample_size; audio_buffer_count++)
		{
			audio_buffer[audio_buffer_count]=0;
		}
	}

	alt_up_audio_write_fifo(audio, audio_buffer, sample_size, ALT_UP_AUDIO_LEFT);
	alt_up_audio_write_fifo(audio, audio_buffer, sample_size, ALT_UP_AUDIO_RIGHT);
}


short convert_to_16 (char a, char b) {
	short c = (unsigned char) a << 8 | (unsigned char) b;

	return c;
}

void open_file(filename) {

	opened_file = alt_up_sd_card_fopen(filename, 0);
}

void reverse_array(int array[], int count) {
	int temp, i;

	for (i = 0; i < count/2; ++i)
	{
		temp = array[i];
		array[i] = array[count-i-1];
		array[count-i-1] = temp;
	}
}

void audio_file_info() {
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

	file_size = (audio_file_size[0] << 8*3 | audio_file_size[1] << 8*2 | audio_file_size[2] << 8 | audio_file_size[3]) + 8;
	printf("file size: %d\n", file_size);
}

void audio_data_load() {
	int i,j;
	unsigned int audio_data_byte[2];

	if(filename == "c.wav")
	{
		//Allocates memory space for buffer
		c_data = (unsigned int *) malloc ((file_size/2) * sizeof(unsigned int));

		printf("Loading c data\n");

		for(i = 0; i < (file_size/2); i++)
		{
			for(j = 0; j < 2; j++)
			{
				audio_data_byte[j] = alt_up_sd_card_read(opened_file) & (0x00FF);
			}

			c_data[i] = convert_to_16(audio_data_byte[1], audio_data_byte[0]);
		}

		printf("Finish loading c data\n");
	}

	else if(filename == "d.wav")
	{
		d_data = (unsigned int *) malloc ((file_size/2) * sizeof(unsigned int));

		printf("Loading d data\n");

		for(i = 0; i < (file_size/2); i++)
		{
			for(j = 0; j < 2; j++)
			{
				audio_data_byte[j] = alt_up_sd_card_read(opened_file) & (0x00FF);
			}

			d_data[i] = convert_to_16(audio_data_byte[1], audio_data_byte[0]);
		}

		printf("Finish loading d data\n");
	}

	else if(filename == "e.wav")
	{
		e_data = (unsigned int *) malloc ((file_size/2) * sizeof(unsigned int));

		printf("Loading e data\n");

		for(i = 0; i < (file_size/2); i++)
		{
			for(j = 0; j < 2; j++)
			{
				audio_data_byte[j] = alt_up_sd_card_read(opened_file) & (0x00FF);
			}

			e_data[i] = convert_to_16(audio_data_byte[1], audio_data_byte[0]);
		}

		printf("Finish loading e data\n");
	}

	else if(filename == "f.wav")
	{
		f_data = (unsigned int *) malloc ((file_size/2) * sizeof(unsigned int));

		printf("Loading f data\n");

		for(i = 0; i < (file_size/2); i++)
		{
			for(j = 0; j < 2; j++)
			{
				audio_data_byte[j] = alt_up_sd_card_read(opened_file) & (0x00FF);
			}

			f_data[i] = convert_to_16(audio_data_byte[1], audio_data_byte[0]);
		}

		printf("Finish loading f data\n");
	}

	else if(filename == "g.wav")
	{
		g_data = (unsigned int *) malloc ((file_size/2) * sizeof(unsigned int));

		printf("Loading g data\n");

		for(i = 0; i < (file_size/2); i++)
		{
			for(j = 0; j < 2; j++)
			{
				audio_data_byte[j] = alt_up_sd_card_read(opened_file) & (0x00FF);
			}

			g_data[i] = convert_to_16(audio_data_byte[1], audio_data_byte[0]);
		}

		printf("Finish loading g data\n");
	}

	else if(filename == "a.wav")
	{
		a_data = (unsigned int *) malloc ((file_size/2) * sizeof(unsigned int));

		printf("Loading a data\n");

		for(i = 0; i < (file_size/2); i++)
		{
			for(j = 0; j < 2; j++)
			{
				audio_data_byte[j] = alt_up_sd_card_read(opened_file) & (0x00FF);
			}

			a_data[i] = convert_to_16(audio_data_byte[1], audio_data_byte[0]);
		}

		printf("Finish loading a data\n");
	}

	else if(filename == "b.wav")
	{
		b_data = (unsigned int *) malloc ((file_size/2) * sizeof(unsigned int));

		printf("Loading b data\n");

		for(i = 0; i < (file_size/2); i++)
		{
			for(j = 0; j < 2; j++)
			{
				audio_data_byte[j] = alt_up_sd_card_read(opened_file) & (0x00FF);
			}

			b_data[i] = convert_to_16(audio_data_byte[1], audio_data_byte[0]);
		}

		printf("Finish loading b data\n");
	}

}

void audio_initialize() {
	audio = alt_up_audio_open_dev(AUDIO_0_NAME);

	alt_up_audio_reset_audio_core(audio);

	audio_buffer = (unsigned int *) malloc (sample_size * sizeof(unsigned int));

	//volumeLCD();
}

void start_audio_interrupt() {
	if(alt_irq_register(AUDIO_0_IRQ, NULL ,audio_isr) == 0)
		printf("irq_register success\n");
	else
		printf("irq_register fail\n");

	printf("Enable Interrupt\n");
	alt_up_audio_enable_write_interrupt(audio);
}

void stop_audio_interrupt() {
	alt_up_audio_disable_write_interrupt(audio);
}

void load_music(char *selectedFile) {
	filename = selectedFile;
	open_file(filename);
	if(filename == "c.wav")
		audio_file_info();
	audio_data_load();
}


void load_audio(void) {
	read_sd();
	load_music("c.wav");
	load_music("d.wav");
	load_music("e.wav");
	load_music("f.wav");
	load_music("g.wav");
	load_music("a.wav");
	load_music("b.wav");
	audio_initialize();
	start_audio_interrupt();
}

void play_sound(int ascii[], int size){
	keys = ascii;
	keySize = size;
	//printf("sound found: %i\n", ascii);
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

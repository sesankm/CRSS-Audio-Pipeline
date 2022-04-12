#include <errno.h> 
#include <dirent.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>


int
main(int argc, char** argv){
	if(argc < 8){
		printf("ERROR: Not enough arguments\n");
		printf("Usage: chunk [input file] [mission] [tape] [historical recorder] [channel] [mission start] [mission end] [output dir name]\n");
		exit(1);
	}

	char orig_file_name[100];
	char* dir_name = argv[8];
	const char* file_in_path;

	sprintf(orig_file_name, "A%s_T%s_%s_CH%s", argv[2], argv[3], argv[4], argv[5]);
	file_in_path = argv[1];

	int h, m, number_of_minutes;
	char* date_buff = malloc(sizeof(char)*10);
	char* date_time = malloc(sizeof(char)*5);
	int date_time_ind=0;
	int day_of_the_year = 0;
	date_buff = strtok(argv[6],"-");

	/* split input date */
	while(date_buff!=NULL)
	{
		if(date_time_ind == 0)
			date_time = date_buff;
		else if (date_time_ind == 1)
			h = atoi(date_buff);
		else
			m = atoi(date_buff);
		date_time_ind++;
		date_buff = strtok(NULL,"-");
	}

	/* populate month and day variables */
	char* month = malloc(sizeof(char)*2);
	char* day = malloc(sizeof(char)*2);
	char* year = malloc(sizeof(char)*4);

	month[0] = date_time[0];
	month[1] = date_time[1];

	day[0] = date_time[2];
	day[1] = date_time[3];

	year[0] = date_time[4];
	year[1] = date_time[5];
	year[2] = date_time[6];
	year[3] = date_time[7];

	/* convert month and days from strings to ints */
	int month_num = atoi(month);
	int day_num = atoi(day);
	int year_num = atoi(year);
	
	/* array for number of days in month */
	int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if(year_num % 4 == 0)
		months[1] = 29;

	for(int i = 0; i < month_num - 1; i++)
		day_of_the_year += months[i];
	day_of_the_year += day_num;

	number_of_minutes = h*60+m;

	// check if directory exists
	DIR* dir = opendir(dir_name);
	if(dir){
		// if it exists exit with error
		printf("ERROR: Directory '%s' already exists\n", dir_name);
		closedir(dir);
		exit(1);
	}
	// if dir doesn't exist
	else if(ENOENT == errno){
		// try creating it 
		if(mkdir(dir_name, 0777) != 0){ // if there's an error when creating, exit
			printf("ERROR: Couldn't create directory: %s", dir_name);
			exit(1);
		}
	}

	SNDFILE* file;
	SF_INFO info;
	short* data;

	// open file for reading
	file = sf_open(file_in_path, SFM_READ, &info);
	// create array for storing data
	data = malloc(sizeof(short) * info.frames * info.channels);

	// read wave file
	printf("Reading...\n");
	if( !sf_readf_short(file, data, info.frames) ){
		printf("chunk: Couldn't read file.");
		exit(-1);
	}

	unsigned long window = info.samplerate * 60 * 30; // this is the number of samples in 30 minutes
	unsigned long ind = 0; // copy 'window' number of elements starting at this index (so 30 minutes worth of samples) to buffer
	short* buffer = malloc(sizeof(short) * window); // save buffer to a file
	int num_original_samples = info.frames; // save total number of samples
	info.frames = (int) window; // edit info struct since we're writing new data
	info.format = SF_FORMAT_WAV|SF_FORMAT_PCM_16; 
	int count = 0; // keeps track of the chunk id
	int done = 0; // bool to break the loop

	while(!done){
		int time_hour;
		if(number_of_minutes >= 24*60) {
			number_of_minutes = number_of_minutes-(24*60);
			day_of_the_year++;
		}

		time_hour = floor(number_of_minutes / 60);
		int time_minute = number_of_minutes % 60;

		char out_file_name[250];
		sprintf(out_file_name, "%s/%s_%d-%02d-%02d_%d-%02d-%02d-%d.wav", dir_name, orig_file_name, day_of_the_year, time_hour, time_minute, day_of_the_year, (int)floor((number_of_minutes + 30) / 60), (int)floor((number_of_minutes + 30) % 60), count);


		/* if the window goes over number of elements, change window size to number of remaining elements */
		if(ind + window > num_original_samples){
			window = num_original_samples - ind;
			info.frames = num_original_samples - ind;
			done = 1;
			printf("Chunks written: %d\n", count);
		}

		/* copy current chunk of file and write to a file  */
		memcpy(buffer, data + ind, window * sizeof(short));
		SNDFILE* outfile = sf_open(out_file_name, SFM_WRITE, &info);
		sf_write_short(outfile, buffer, window);
		sf_write_sync(outfile);
		sf_close(outfile);

		/* move to the next chunk and increment time */ 
		ind += window;
		count++;
		number_of_minutes += 30;
	}
	sf_close(file);
}

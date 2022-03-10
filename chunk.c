#include <errno.h> 
#include <dirent.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int
main(int argc, char** argv){
	if(argc < 8){
		printf("ERROR: Not enough arguments\n");
		printf("Usage: chunk [input file] [mission] [tape] [historical recorder] [cannel] [mission start] [mission end] [output dir name]\n");
		exit(1);
	}

	char orig_file_name[100];
	char* dir_name = argv[8];
	const char* mission_start;
	const char* mission_end;
	const char* file_in_path;

	sprintf(orig_file_name, "A%s_T%s_HR%s_CH%s", argv[2], argv[3], argv[4], argv[5]);
	mission_start = argv[6];
	mission_end = argv[7];
	file_in_path = argv[1];

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
		char out_file_name[250];
		sprintf(out_file_name, "%s/%s_%s_%s-%d.wav", dir_name, orig_file_name, mission_start, mission_end, count);

		if(ind + window > num_original_samples){
			window = num_original_samples - ind;
			info.frames = num_original_samples - ind;
			done = 1;
			printf("Chunks written: %d\n", count);
		}

		memcpy(buffer, data + ind, window * sizeof(short));

		SNDFILE* outfile = sf_open(out_file_name, SFM_WRITE, &info);
		sf_write_short(outfile, buffer, window);
		sf_write_sync(outfile);
		sf_close(outfile);

		ind += window;
		count++;
	}
	sf_close(file);
}

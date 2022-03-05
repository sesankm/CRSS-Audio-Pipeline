#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>
#include <math.h>

typedef struct {
	short* data;
	long count;
} WaveData;

WaveData
downsample(short* data, long arr_size, int original_sample_rate, int new_sample_rate){
	unsigned long long new_size;
	double ratio, ind;
	short* out;
	long count;
	
	new_size = (unsigned long long)arr_size * (unsigned long long)new_sample_rate / (unsigned long long)original_sample_rate;
	ratio = original_sample_rate / new_sample_rate;
	count = 0;
	ind = 0;

	out = malloc(sizeof(short) * new_size);
	if(out == NULL) printf("malloc error\n");

	while(ind < arr_size){
		int counter = 0;
		short val = 0;
		while(counter < ratio){
			val += data[(long)ind];
			counter++;
			ind++;
		}

		if(counter != ratio)
			val += data[(long)ind + 1] / 2;
		val = (short)val / ratio;
		out[count] = val;
		count++;
	}

	return (WaveData) {out, new_size};
}

int
main(int argc, char** argv){
	if(argc < 3){
		printf("ERROR: Not enough arguments\n");
		printf("Usage: downsample [input file path] [output file path]\n");
		exit(1);
	}

	const char* file_in_path = argv[1];
	const char* file_out_path = argv[2];

	SNDFILE* file;
	SF_INFO info;
	short* data;


	// read input file
	file = sf_open(file_in_path, SFM_READ, &info);

	// alloc space for array of samples
	data = malloc(sizeof(short) * info.frames * info.channels);

	printf("Reading...\n");
	// read the data
	if( !sf_readf_short(file, data, info.frames) ){
		printf("Couldn't read file.");
		exit(-1);
	}

	printf("Downsampling...\n");
	WaveData output = downsample(data, info.frames, info.samplerate, 8000);

	info.samplerate = 8000;
	info.frames = output.count;
	/* info.channels = 1; */

	/* // output path */
	SNDFILE* outfile = sf_open(file_out_path, SFM_WRITE, &info);
	// write data to file
	sf_writef_short(outfile, output.data, output.count);
	sf_write_sync(outfile);

	// close file
	sf_close(outfile);
	sf_close(file);

	printf("Finished downsampling.\n");
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

typedef struct {
	double* data;
	int count;
} WaveData;

WaveData
peak_normalize(short* data, int arr_size, short max){
	double* out = malloc(sizeof(double) * arr_size);
	double factor = 1.0 / max;

	for(int j = 0; j < arr_size; j++)
		out[j] = (double)factor * data[j];

	return (WaveData) {out, arr_size};
}

WaveData
downsample(double* data, int arr_size, int original_sample_rate, int new_sample_rate){
	unsigned long long new_size;
	double ratio, ind;
	double* out;
	int count;
	
	new_size = (unsigned long long)arr_size * (unsigned long long)new_sample_rate / (unsigned long long)original_sample_rate;
	ratio = original_sample_rate / new_sample_rate;
	count = 0;
	ind = 0;

	out = malloc(sizeof(double) * new_size);

	while(ind < arr_size){
		if(!(ind == (int)ind))
			out[count] = (data[(int)ind] + data[(int)ind + 1]) / 2;
		else
			out[count] = data[(int)ind];

		ind += ratio;
		count++;
	}

	return (WaveData) {out, new_size};
}

int
main(int argc, char** argv){
	if(argc < 3){
		printf("ERROR: Not enough arguments\n");
		printf("Usage: normalize [input file path] [output file path]\n");
		exit(1);
	}

	const char* file_in_path = argv[1];
	const char* file_out_path = argv[2];

	SNDFILE* file;
	SF_INFO info;
	short* data;
	short max;

	short top_two[2] = {0, 0};
	int top_two_inds[2] = {0, 0};

	// read input file
	file = sf_open(file_in_path, SFM_READ, &info);

	// alloc space for array of samples
	data = malloc(sizeof(short) * info.frames * info.channels);
	max = 0;

	printf("Reading...\n");
	// read the data
	if( !sf_readf_short(file, data, info.frames) ){
		printf("Couldn't read file.");
		exit(-1);
	}

	// find max
	for(int i = 0; i < info.frames; i++)
		if(data[i] > top_two[0]){
			top_two[0] = data[i];
			top_two_inds[0] = i;
		}
		else if(data[i] > top_two[1]){
			top_two[1] = data[i];
			top_two_inds[1] = i;
		}
		/* if(data[i] > max) */
		/* 	max = data[i]; */

	if(top_two[0] - top_two[1] > 900){
		max = top_two[1];
		printf("%d\n", top_two_inds[1] / info.samplerate);
	}
	else{
		printf("%d\n", top_two_inds[0] / info.samplerate);
		max = top_two[0];
	}

	printf("Normalizing...\n");
	WaveData output = peak_normalize(data, info.frames, max);

	printf("Downsampling...\n");
	output = downsample(output.data, info.frames, info.samplerate, 8000);

	info.samplerate = 8000;
	info.frames = output.count;

	// output path
	SNDFILE* outfile = sf_open(file_out_path, SFM_WRITE, &info);
	// write data to file
	sf_write_double(outfile, output.data, output.count);
	sf_write_sync(outfile);

	// close file
	sf_close(outfile);
	sf_close(file);
	// free data array
	free(data);

	printf("Done\n");
	return 0;
}

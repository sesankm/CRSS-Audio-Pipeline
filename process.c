#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>

int
main(int argc, char** argv){
	/* Required Arguments in this order: 
	 *  Input file name
	 *  Mission number
	 *  Tape number
	 *  Recorder number
	 *  Channel number
	 *  Mission start in this format: "[day]-[hour]-[minute]-[seconds]"
	 *  Mission end in this format: "[day]-[hour]-[minute]-[seconds]" */

	if(argc < 7){
		printf("ERROR: Not enough arguments.\n");
		printf("Usage: ./process [input file] [mission number] [tape number] [recorder number] [channel number] [mission start] [mission end].\n");
		exit(1);
	}

	int n = fork();
	if(n == 0)
		execl("./normalize", "./normalize", argv[1], "./normed_audio.wav", (char*) NULL);
	else
		wait(NULL);
	
	n = fork();
	if(n == 0){
		/* execl("/usr/bin/sox", "/usr/bin/sox", "./normed_audio.wav", "-r", "8000", "./downsampled_audio.wav", (char*) NULL); */
		execl("./downsample", "./downsample", "./normed_audio.wav", "./downsampled_audio.wav", (char*) NULL);
	}
	else{
		wait(NULL);
	}

	n = fork();
	if(n == 0)
		execl("./chunk", "./chunk", "./downsampled_audio.wav", argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], (char*) NULL);
	else
		wait(NULL);
	exit(0);
}

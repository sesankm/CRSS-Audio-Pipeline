#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int get_channel_names(char* channel_names_text_file, char** channel_names){
	char* line = malloc(sizeof(char) * 500);
	FILE* fp;
	size_t len;
	int count = 0;

	fp = fopen(channel_names_text_file, "r");

	while(getline(&line, &len, fp) != -1) {
		channel_names[count] = malloc(sizeof(char) * 400);
		if(line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';
		strcpy(channel_names[count], line);
		count++;
    }

	return count;
}

int
main(int argc, char** argv){
	/* Required Arguments in this order: 
	 *  Directory containing channels
	 *  Mission number
	 *  Tape number
	 *  Recorder number
	 *  Mission start in this format: "[YYYYMMDD]-[HH]-[MM]"
	 *  Mission end in this format: "[YYYYMMDD]-[HH]-[MM]" */

	char* base_dir = argv[1];
	char* channel_names_text_file = "channels.txt";
	char** channel_names = malloc(sizeof(char*) * 32);
	int num_channels = get_channel_names(channel_names_text_file, channel_names);

	if(argc < 6){
		printf("ERROR: Not enough arguments.\n");
		printf("Usage: ./process [input directory] [mission number] [tape number] [recorder number] [YYYYMMDD-HH-MM (start time)] [YYYYMMDD-HH-MM (end time)].\n");
		exit(1);
	}

	for(int i = 9; i < num_channels; i++){
		char file_name[500];
		char output_dir_name[500];

		if(base_dir[strlen(base_dir) - 1] == '/')
			base_dir[strlen(base_dir) - 1] = '\0';

		sprintf(file_name, "%s/CH%d.wav", base_dir, i);
		sprintf(output_dir_name, "A%s-%s", argv[2],channel_names[i-1]);

		printf("%s.....%s\n", file_name, output_dir_name);

		int n = fork();
		if(n == 0)
			execl("./normalize", "./normalize", file_name, "./normed_audio.wav", (char*) NULL);
		else
			wait(NULL);
	
		n = fork();
		if(n == 0){
			execl("/usr/bin/sox", "/usr/bin/sox", "./normed_audio.wav", "-r", "8000", "./downsampled_audio.wav", (char*) NULL);
			/* execl("./downsample", "./downsample", "./normed_audio.wav", "./downsampled_audio.wav", (char*) NULL); */
		}
		else{
			wait(NULL);
		}

		n = fork();
		if(n == 0)
			execl("./chunk", "./chunk", "./downsampled_audio.wav", argv[2], argv[3], argv[4], channel_names[i-1], argv[5], argv[6], output_dir_name, (char*) NULL);
		else
			wait(NULL);
		break;
	}

	exit(0);
}

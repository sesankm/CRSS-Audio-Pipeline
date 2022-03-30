#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(int argc, char *argv[])
{
    int h, m, timecode;
	if(argc < 3 || argc > 3)
	{
		printf("Arguments supplied are insufficient or too much.\n");
		exit(0);
	}
	else {
	h = atoi(argv[1]);
	m = atoi(argv[2]);
	timecode = h*60+m;
	for(int i = 0; i < 5; i++)
	{
		
		int temp1;
		temp1 = floor(timecode / 60);
		int temp2 = timecode % 60;
		if((timecode%60) < 10)
		  printf("%d:0%d\n", temp1, temp2);
	  	else
        printf("%d:%d\n", temp1, temp2);
		timecode = timecode+30;
	}
	}	
	
	return 0;
}

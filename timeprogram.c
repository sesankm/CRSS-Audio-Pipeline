#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int sec, h, m;
    char time[100];
	if(argc < 3)
	{
		printf("Arguments supplied are insufficient.\n");
		exit(0);
	}
	else {
	h = atoi(argv[1]);
	m = atoi(argv[2]);
    //printf("Input time: ");
    //scanf("%s", time);
	if (sscanf(time, "%d:%d", &h, &m) >= 2){
	   sec = h*3600 + m*60;
	}
	for(int count = 0; count < 5; count++){
	  sec = sec + 1800;
	  h = (sec/3600);
	  m = (sec-(3600*h))/60;
	 int nDigits ;
	  if(m < 10)
		  printf("%d:0%d\n", h, m);
	  else
        printf("%d:%d\n", h, m);		    
	}
	}	
	
	return 0;
}

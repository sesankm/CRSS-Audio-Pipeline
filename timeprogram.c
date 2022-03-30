#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
int main(int argc, char *argv[])
{
	//DDDD-HH-MM
	//0329-23-10
	int h, m, timecode;
	if(argc!=2)
	{
		printf("Arguments supplied are insufficient or too much.\n");
		exit(0);
	}
	char* buffer = malloc(sizeof(char)*10);
	buffer = strtok(argv[1],"-");
	int ind=0;
	char* date_time = malloc(sizeof(char)*5);
	while(buffer!=NULL)
	{
		
		if(ind == 0)
			date_time = buffer;
		else if (ind == 1)
			h = atoi(buffer);
		else 
			m = atoi(buffer);
		ind++;
		buffer = strtok(NULL,"-");
	}
	

	char* month = malloc(sizeof(char)*3);
	char* day = malloc(sizeof(char)*3);

	month[0] = date_time[0];
	month[1] = date_time[1];
	day[0] = date_time[2];
	day[1] = date_time[3];

	int month_num = atoi(month);
	int day_num = atoi(day);


	timecode = h*60+m;
	for(int i = 0; i < 5; i++)
	{
		
		int temp1;
		if(timecode >= 24*60)
		{
			timecode = timecode-(24*60);
			day_num++;
			if(month_num == 1 && day_num > 31)
			{
    			month_num += 1;
    			day_num = 1;
  			}
			else if(month_num == 2 && day_num > 28)
			{
    			month_num += 1;
    			day_num = 1;
			}
			else if(month_num == 3 && day_num > 31)
			{
				month_num += 1;
				day_num = 1;
			}
			else if(month_num == 4 && day_num > 30)
			{
				month_num += 1;
				day_num = 1;
			}
			else if(month_num == 5 && day_num > 31)
			{
				month_num += 1;
				day_num = 1;
			}
			else if(month_num == 6 && day_num > 30)
			{
				month_num += 1;
				day_num = 1;
			}
			else if(month_num == 7 && day_num > 31)
			{
				month_num += 1;
				day_num = 1;
			}
			else if(month_num == 8 && day_num > 31)
			{
				month_num += 1;
				day_num = 1;
			}
			else if(month_num == 9 && day_num > 30)
			{
				month_num += 1;
				day_num = 1;
			}
			else if(month_num == 10 && day_num > 31)
			{
				month_num += 1;
				day_num = 1;
			}
			else if(month_num == 11 && day_num > 30)
			{
				month_num += 1;
				day_num = 1;
			}
			else if(month_num == 12 && day_num > 31)
			{
				month_num = 1;
				day_num = 1;
			}
				
		}

		temp1 = floor(timecode / 60);
		int temp2 = timecode % 60;
		printf("%02d-%02d_%02d:%02d\n", month_num, day_num, temp1, temp2);
		timecode = timecode+30;

	}	

	
	return 0;
}

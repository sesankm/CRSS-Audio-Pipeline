norm:
	gcc -o process process.c -g && gcc -o normalize normalize.c -lm -ldl -lsndfile -Wall -g &&  gcc -o downsample downsample.c -lsndfile -Wall && gcc -o chunk chunk.c -lsndfile -Wall
clean:
		rm -f chunk normalize process downsample

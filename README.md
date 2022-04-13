# CRSS Audio Processing Pipeline

## Requirements
- [`libsndfile`](https://github.com/libsndfile/libsndfile)
- [ffmpeg](https://www.ffmpeg.org/download.html)
- tested on Debian

## Running the pipeline
- `make`
- `./process [input] [mission] [tape no.] [recorder no.] [channel no.] [mission start] [mission end]`
- mission start and ends should be in this format:
  - `[day]-[hour]-[minute]`
  - [day] should be in this format: `MMDDYYYY`
- example: `./process ./Audio\ Files/ 8 213 HR1L 03221999-03-24 03221999-20-24`
- this runs normalize.c, then saves normalized and downsampled file in `normed_audio.wav`
- after normalizing, it runs chunk.c with `normed_audio.wav` as input

## Chunking
- `make`
- `./chunk [input] [mission] [tape no.] [recorder no.] [channel no.] [mission start] [mission end]`
- mission start and ends should be in this format:
  - `[day]-[hour]-[minute]-[seconds]`
- creates a directory called `A[mission number]_chunks` and saves normalized, downsampled, 30 minute chunks in this folder
  - assumes that this directory doesn't already exist, throws an error if it already exists

## Normalizing and Downsampling
- run `make` to build
- `./normalize [input file path] [output file path]`
- normalizes and downsamples wav file

## 
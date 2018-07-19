
#pragma once

#include <limits>

#define SAMPLE_RATE  long(44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     long( 60 * 60 * 24) //最大24時間 
#define NUM_CHANNELS    (2)
#define DITHER_FLAG     (0)
#define WRITE_TO_FILE   (0)

#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#elif 0
#else
#endif

typedef struct{
    long long     recordFrameIndex;  /* Index into sample array. */
    long long     playFrameIndex;
    long long     maxFrameIndex;
    int           recontinue;
    cv::Mat       *drawPlace;
    SAMPLE        *recordedSamples;
}paTestData;

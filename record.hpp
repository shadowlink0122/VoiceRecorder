
#pragma once

#include <stdlib.h>
#include <portaudio.h>
#include "record_params.hpp"

static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData ){

    paTestData *data = (paTestData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &data->recordedSamples[data->recordFrameIndex * NUM_CHANNELS];
    long framesToCalc;
    long long i;
    int finished;
    long long framesLeft = data->maxFrameIndex - data->recordFrameIndex;

    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( data->recontinue != RECORD || framesLeft < framesPerBuffer ){
        framesToCalc = framesLeft;
        finished = paComplete;
        return finished;
    }else{
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if( data->recontinue != RECORD || inputBuffer == NULL ){
        for( i=0; i<framesToCalc; i++ ){
            *wptr++ = SAMPLE_SILENCE;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
        }
        framesToCalc = 0;
    }else{
        for( i=0; i<framesToCalc; i++ ){
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
    }

    data->recordFrameIndex += framesToCalc;
    // std::cout << "recordIndex : " << data->recordFrameIndex << std::endl;
    return finished;
}

int callRecord(paTestData *data){
    PaStreamParameters  inputParameters,outputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    long long           i;
    long long           totalFrames;
    long long           numSamples;
    long long           numBytes;
    SAMPLE              max, val;
    double              average;

    fflush(stdout);

    data->maxFrameIndex = totalFrames = (NUM_SECONDS * SAMPLE_RATE); /* Record for a one hour. */
    data->recordFrameIndex = data->playFrameIndex = 0;
    numSamples = totalFrames * NUM_CHANNELS;
    numBytes = numSamples * sizeof(SAMPLE);

    if( data->recordedSamples )       /* Sure it is NULL or valid. */
    free( data->recordedSamples );    // Clear sound memory
    data->recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
    if( data->recordedSamples == NULL ){
        printf("Could not allocate record array.\n");
        goto done;
    }

    err = Pa_Initialize();
    if( err != paNoError ) goto done;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto done;
    }
    inputParameters.channelCount = 2;                    /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  /* &outputParameters, */
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              recordCallback,
              data );
    if( err != paNoError ) goto done;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto done;
    // printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);

    // ここで1秒ごとにデータを取ってくる。
    while( ( err = Pa_IsStreamActive( stream ) ) == 1 ){
        Pa_Sleep(1000);
        printf("index = %lld\n", data->recordFrameIndex ); fflush(stdout);
    }
    if( err < 0 ) goto done;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto done;

done:
    Pa_Terminate();
    data->recontinue = STOP;
    puts("FINISH!");
    return err;
}


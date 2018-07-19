
#pragma once

#include <opencv/cv.hpp>
#include <portaudio.h>
#include "record_params.hpp"

static int playCallback( const void *inputBuffer, void *outputBuffer,
						 unsigned long framesPerBuffer,
						 const PaStreamCallbackTimeInfo* timeInfo,
						 PaStreamCallbackFlags statusFlags,
						 void *userData ){
	
	paTestData *data = (paTestData*)userData;
	SAMPLE *rptr = &data->recordedSamples[data->playFrameIndex * NUM_CHANNELS];
	SAMPLE *wptr = (SAMPLE*)outputBuffer;
	long long i;
	int finished;
	long long framesLeft = data->recordFrameIndex - data->playFrameIndex;

	(void) inputBuffer; /* Prevent unused variable warnings. */
	(void) timeInfo;
	(void) statusFlags;
	(void) userData;

	// std::cout << data->recordFrameIndex << std::endl;
	// std::cout << data->playFrameIndex << std::endl << std::endl;

	if( data->recontinue == STOP || data->recordedSamples == NULL)return paComplete;

	if( data->recontinue != PLAY || framesLeft < framesPerBuffer ){
		/* final buffer... */
		for( i=0; i<framesLeft; i++ ){
			// std::cout << *wptr << std::endl;
			*wptr++ = *rptr++;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
		}
		for( ; i<framesPerBuffer; i++ ){
			*wptr++ = 0;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = 0;  /* right */
		}
		data->playFrameIndex += framesLeft;
		finished = paComplete;
	}
	else{
		for( i=0; i<framesPerBuffer; i++ ){
			std::cout << *wptr << std::endl;
			*wptr++ = *rptr++;  /* left */
			if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
		}
		data->playFrameIndex += framesPerBuffer;
		finished = paContinue;
	}
	return finished;
}

int callPlay(paTestData *data){
	PaStreamParameters  outputParameters;
	PaError             err = paNoError;
	PaStream*           stream;

	if(data->recordedSamples == NULL){
		data->recontinue = STOP;
		return !err;
	}

	data->playFrameIndex = 0;

	err = Pa_Initialize();

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr,"Error: No default output device.\n");
		goto done;
	}
	outputParameters.channelCount = 2;                     /* stereo output */
	outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	// printf("\n=== Now playing back. ===\n"); fflush(stdout);
	err = Pa_OpenStream(
			  &stream,
			  NULL, /* no input */
			  &outputParameters,
			  SAMPLE_RATE,
			  FRAMES_PER_BUFFER,
			  paClipOff,      /* we won't output out of range samples so don't bother clipping them */
			  playCallback,
			  data );
	if( err != paNoError ) goto done;

	if( stream ){

		err = Pa_StartStream( stream );
		if( err != paNoError ) goto done;
		
		// printf("Waiting for playback to finish.\n"); fflush(stdout);

		while( ( err = Pa_IsStreamActive( stream ) ) == 1 );
		if( err < 0 ) goto done;
		
		err = Pa_CloseStream( stream );
		if( err != paNoError ) goto done;
		
		printf("Done.\n"); fflush(stdout);
	}

done:
	Pa_Terminate();
	if( err != paNoError ){
		fprintf( stderr, "An error occured while using the portaudio stream\n" );
		fprintf( stderr, "Error number: %d\n", err );
		fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
		err = 1;          /* Always return 0 or 1, but no other return codes. */
	}
	data->recontinue = STOP;
	cvDestroyWindow("Graph");
	return err;
}

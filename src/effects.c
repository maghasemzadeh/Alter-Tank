//
// Created by amin on 1/3/19.
//

#include "effects.h"
#include "structs.h"


void audioCallback(void* userData, Uint8* stream, int streamLength) {
    AudioData* audio = (AudioData*)userData;

    if (audio->length == 0)
    {
        return;
    }

    Uint32 length = (Uint32)streamLength;

    length = (length > audio->length ? audio->length : length);

    SDL_memcpy(stream, audio->position, length);

    audio->position += length;
    audio->length -= length;
}

void play_sound_effect() {
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec wavSpec;
    Uint8* wavStart;
    Uint32 wavLength;
    char* filePath = "Explosion+1.wav";

    if(SDL_LoadWAV(filePath, &wavSpec, &wavStart, &wavLength) == NULL)
    {
        printf("Error: file could not be loaded as an audio file.\n") ;
    }

    AudioData audio;
    audio.position = wavStart;
    audio.length = wavLength;

    wavSpec.callback = audioCallback;
    wavSpec.userdata = &audio;

    SDL_AudioDeviceID audioDevice;
    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE);

    if (audioDevice == 0)
    {
        printf("Error: %s\n" , SDL_GetError() );
        return;
    }

    SDL_PauseAudioDevice(audioDevice, 0);

    while (audio.length > 0)
    {
        SDL_Delay(100);
    }

    SDL_CloseAudioDevice(audioDevice);
    SDL_FreeWAV(wavStart);

}


#include <SDL2/SDL.h>

#define MUS_PATH "Explosion+1.wav"

// prototype for our audio callback
// see the implementation for more information

// variable declarations
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play


// audio callback function
// here you have to copy the data of your audio buffer into the
// requesting audio buffer (stream)
// you should only copy as much as the requested length (len)
void my_audio_callback(void *userdata, Uint8 *stream, int len) {

    if (audio_len ==0)
        return;

    len = ( len > audio_len ? audio_len : len );
    //SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

    audio_pos += len;
    audio_len -= len;
}

//
///*
//** PLAYING A SOUND IS MUCH MORE COMPLICATED THAN IT SHOULD BE
//*/
//int main(int argc, char* argv[]){
//
//    // Initialize SDL.
//    if (SDL_Init(SDL_INIT_AUDIO) < 0)
//        return 1;
//
//    // local variables
//    static Uint32 wav_length; // length of our sample
//    static Uint8 *wav_buffer; // buffer containing our audio file
//    static SDL_AudioSpec wav_spec; // the specs of our piece of music
//
//
//    /* Load the WAV */
//    // the specs, length and buffer of our wav are filled
//    if( SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL ){
//        return 1;
//    }
//    // set the callback function
//    wav_spec.callback = my_audio_callback;
//    wav_spec.userdata = NULL;
//    // set our global static variables
//    audio_pos = wav_buffer; // copy sound buffer
//    audio_len = wav_length; // copy file length
//
//    /* Open the audio device */
//    if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
//        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
//        exit(-1);
//    }
//
//    /* Start playing */
//    SDL_PauseAudio(0);
//
//    // wait until we're don't playing
//    while ( audio_len > 0 ) {
//        SDL_Delay(100);
//    }
//
//    // shut everything down
//    SDL_CloseAudio();
//    SDL_FreeWAV(wav_buffer);
//
//}

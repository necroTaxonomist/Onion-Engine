#ifndef ONIONSOUND_H
#define ONIONSOUND_H

#include <portaudio.h>
#include <sndfile.h>
#include <string>

class OnionSound
{
    public:
        static int numInstances;

        OnionSound(std::string filename);
        ~OnionSound();

        void play(bool looping = false);
        void stop();

        std::string getFilename() { return fn; };

        bool isPlaying();
        bool mustBeClosed();

    private:
        PaStream* stream;
        SNDFILE* file;
        SF_INFO sfinfo;

        std::string fn;
        bool loop;
        bool playing;
        bool streamEnded;

        static int callback(const void *input,
                                    void *output,
                                    unsigned long frameCount,
                                    const PaStreamCallbackTimeInfo* timeInfo,
                                    PaStreamCallbackFlags statusFlags,
                                    void *userData)
        {
            return ((OnionSound*)userData)->callbackFunction(input,output,frameCount);
        };
        int callbackFunction(const void *input,
                             void *output,
                             unsigned long frameCount);

        static void streamFinished(void* userData)
        {
            ((OnionSound*)userData)->streamFinishedFunction();
        };
        void streamFinishedFunction();
};

#endif // ONIONSOUND_H

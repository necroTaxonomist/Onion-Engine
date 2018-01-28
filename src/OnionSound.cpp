#include "OnionSound.h"

int OnionSound::numInstances = 0;

OnionSound::OnionSound(std::string filename):
    fn(filename),
    loop(false),
    playing(false),
    streamEnded(true)
{
    /*file = sf_open(filename, SFM_READ, &sfinfo);
    printf("%s: %d samples, %d sps,%d channels\n",
           filename,
            (int)sfinfo.frames,
            sfinfo.samplerate,
            sfinfo.channels);*/
    ++numInstances;
}

OnionSound::~OnionSound()
{
    --numInstances;
}

void OnionSound::play(bool looping)
{
    file = sf_open(fn.c_str(), SFM_READ, &sfinfo);

    loop = looping;

    PaStreamParameters parameters;

    parameters.device = Pa_GetDefaultOutputDevice();
    if (parameters.device == paNoDevice)
    {
        fprintf(stderr, "you fuckin dont even have a device you fucker\n");
        return;
    }
    parameters.channelCount = sfinfo.channels;
    parameters.sampleFormat = paInt16;
    parameters.suggestedLatency = Pa_GetDeviceInfo(parameters.device)->defaultLowOutputLatency;
    parameters.hostApiSpecificStreamInfo = NULL;

    Pa_OpenStream(&stream,
                  NULL,
                  &parameters,
                  sfinfo.samplerate,
                  paFramesPerBufferUnspecified,
                  paClipOff,
                  &OnionSound::callback,
                  this);
    Pa_SetStreamFinishedCallback(stream, &OnionSound::streamFinished);

    playing = true;
    streamEnded = false;

    Pa_StartStream(stream);
}

void OnionSound::stop()
{
    Pa_AbortStream(stream);

    Pa_CloseStream(stream);

    sf_close(file);

    playing = false;
    streamEnded = true;
}

bool OnionSound::isPlaying()
{
    return playing;
}
bool OnionSound::mustBeClosed()
{
    return playing && streamEnded;
}

int OnionSound::callbackFunction(const void *input,
                                 void *output,
                                 unsigned long frameCount)
{
    short* out = (short*)output;

    #pragma warning( suppress : 4244 )
    int bytesRead = sf_read_short(file, out, sfinfo.channels * frameCount);

    if (bytesRead != 0)
        return paContinue;
    else
    {
        if (loop)
        {
            //DO LOOPY THINGS HERE
            file = sf_open(fn.data(), SFM_READ, &sfinfo);
            return paContinue;
        }
        else
        {
            //DO STOPPY THINGS HERE
            //printf("\nThe fuck is this shit");
            return paComplete;
        }
    }
}

void OnionSound::streamFinishedFunction()
{
    //printf("\nIT'S OVER!!!");
    streamEnded = true;
}

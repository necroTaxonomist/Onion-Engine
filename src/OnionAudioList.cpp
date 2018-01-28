#include "OnionAudioList.h"

int OnionAudioList::numInstances = 0;

OnionAudioList::OnionAudioList()
{
    Pa_Initialize();
    ++numInstances;
}

OnionAudioList::~OnionAudioList()
{
    printf("AUDIO LIST DESTRUCTOR STARTED\n");
    Pa_Terminate();
    sounds.clear();
    printf("AUDIO LIST DESTRUCTOR FINISHED\n");
    --numInstances;
}

OnionSound* OnionAudioList::load(std::string filename)
{
    int index = findFile(filename);

    if (index >= 0)
        return &sounds.at(index);
    else
    {
        sounds.push_back(OnionSound(filename));
        return &sounds.at(sounds.size()-1);
    }
}
void OnionAudioList::dealloc(std::string filename)
{
    int index = findFile(filename);
    if (index != -1)
    {
        sounds.erase(sounds.begin()+index);
    }
}

void OnionAudioList::play(std::string filename, bool looping)
{
    OnionSound* playing = load(filename);

    if (playing->isPlaying())
        playing->stop();

    playing->play(looping);

}
void OnionAudioList::stop(std::string filename)
{
    if (findFile(filename) == -1)
        return;
    else
    {
        OnionSound* playing = load(filename);
        if (playing->isPlaying())
            playing->stop();
    }
}
void OnionAudioList::toggle(std::string filename, bool looping)
{
    OnionSound* playing = load(filename);
    bool current = playing->isPlaying();

    if (current)
    {
        //printf("\nIt stopped.");
        playing->stop();
    }
    else
    {
        //printf("\nIt played.");
        playing->play(looping);
    }
}

void OnionAudioList::tidy()
{
    for (int i = 0; i < sounds.size(); ++i)
    {
        if (sounds.at(i).mustBeClosed())
            sounds.at(i).stop();
    }
}

int OnionAudioList::findFile(std::string filename)
{
    //printf("\nSEARCHING %d items...",sounds.size());
    for (int i = 0; i < sounds.size(); ++i)
    {
        //printf("\n%s vs %s...",sounds.at(i).getFilename(), filename);
        if (filename.compare(sounds.at(i).getFilename()) == 0)
        {
            //printf("SAME FILE");
            return i;
        }
    }
    //printf("UNSAME FILE");
    return -1;
}

#ifndef ONIONAUDIOLIST_H
#define ONIONAUDIOLIST_H

#include "OnionSound.h"
#include <vector>

class OnionAudioList
{
    public:
        static int numInstances;

        OnionAudioList();
        ~OnionAudioList();

        OnionSound* load(std::string filename);
        void dealloc(std::string filename);

        void play(std::string filename, bool looping = false);
        void stop(std::string filename);
        void toggle(std::string filename, bool looping = false);

        void tidy();

    private:
        std::vector<OnionSound> sounds;

        int findFile(std::string filename);
};

#endif // ONIONAUDIOLIST_H

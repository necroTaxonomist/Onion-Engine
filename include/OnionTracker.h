#ifndef ONIONTRACKER_H
#define ONIONTRACKER_H

#include "OnionBase.h"

class OnionTracker
{
    public:
        OnionTracker(unsigned long long maxSizeIn = 300000000);
        ~OnionTracker();

        void* alloc(void* o);
        void dealloc(void* o);

    private:
        struct Block
        {
            void* point;
            unsigned long size;
            Block(void* pointIn);
        };

        std::vector<Block> blocks;
};

#endif // ONIONTRACKER_H

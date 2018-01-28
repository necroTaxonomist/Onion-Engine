#include "OnionTracker.h"

#include <cmath>

OnionTracker::OnionTracker(unsigned long long maxSizeIn)
{
}

OnionTracker::~OnionTracker()
{
    //dtor
}

void* OnionTracker::alloc(void* o)
{
    blocks.push_back(Block(o));
}

void OnionTracker::dealloc(void* o)
{
    for (int i = 0; i < blocks.size(); ++i)
    {
        if (blocks.at(i).point == o)
        {
            blocks.erase(blocks.begin()+i);
            return;
        }
    }
}

OnionTracker::Block::Block(void* pointIn):
    point(pointIn)
{
    size = sizeof(point);
}

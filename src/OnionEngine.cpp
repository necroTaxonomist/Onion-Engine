#include "OnionEngine.h"

long long Onion::totalSize()
{
    long long size = 0;
    size += OnionAudioList::numInstances * sizeof(OnionAudioList);
    size += OnionBuffer::numInstances * sizeof(OnionBuffer);
    size += OnionDataSeries::numInstances * sizeof(OnionDataSeries);
    size += OnionHandler::numInstances * sizeof(OnionHandler);
    size += OnionLetter::numInstances * sizeof(OnionLetter);
    size += OnionSound::numInstances * sizeof(OnionSound);
    size += OnionStringList::numInstances * sizeof(OnionStringList);
    size += OnionSurface::numInstances * sizeof(OnionSurface);
    size += OnionTextureList::numInstances * sizeof(OnionTextureList);
    size += OnionTextureList::containedData;
    size += OnionWindow::numInstances * sizeof(OnionWindow);
    return size;
}

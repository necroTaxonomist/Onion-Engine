#include "OnionDataSeries.h"

#include <cstdio>

int OnionDataSeries::numInstances = 0;

OnionDataSeries::OnionDataSeries():
    iter(-1),
    length(0),
    looping(false)
{
    ++numInstances;
}

OnionDataSeries::OnionDataSeries(int len):
    iter(-1),
    length(len),
    looping(false)
{
    data = new unsigned char[length];
    ++numInstances;
}

OnionDataSeries::OnionDataSeries(std::string filename):
    looping(false)
{
    load(filename);
    ++numInstances;
}

OnionDataSeries::OnionDataSeries(unsigned char* dataIn, int dataLength):
    iter(-1),
    length(dataLength),
    data(dataIn),
    looping(false)
{
    ++numInstances;
}

OnionDataSeries::~OnionDataSeries()
{
    delete[] data;
    --numInstances;
}

void OnionDataSeries::load(std::string filename)
{
    #pragma warning( suppress : 4996 )
    FILE *file = fopen(filename.c_str(), "rb");

    if (file == NULL)
    {
        printf("Can't find %s. Dunno where it went.\n", filename.c_str());
        length = 0;
        data = NULL;
        return;
    }

    fseek(file, 0, SEEK_END);

    length = ftell(file);
    data = new unsigned char[length];

    fseek(file, 0, SEEK_SET);
    fread(data, 1, length, file);
    fclose(file);

    iter = -1;
}

void OnionDataSeries::setByte(int index, unsigned char value)
{
    data[index] = value;
}

OnionDataSeries* OnionDataSeries::segment(int startPos, int endPos)
{
    OnionDataSeries* returnSeries = new OnionDataSeries(endPos - startPos);
    for (int i = 0; i < endPos - startPos; ++i)
    {
        returnSeries->setByte(i, at(i + startPos));
    }
    return returnSeries;
}

void OnionDataSeries::reset()
{
    iter = -1;
}

void OnionDataSeries::set(int index)
{
    iter = index;
}

unsigned char OnionDataSeries::at(int index)
{
    if (data != NULL && index >= 0 && index < length)
        return data[index];
    else
        return 0;
}

int OnionDataSeries::size()
{
    return length;
}

bool OnionDataSeries::done()
{
    if (data == NULL)
        return true;

    if (looping)
    {
        if (iter >= length - 1)
            reset();
        return false;
    }
    else
        return iter >= length - 1;
}

void OnionDataSeries::back(int amount)
{
    iter -= amount;
}

void OnionDataSeries::skip(int amount)
{
    iter += amount;
}

unsigned char OnionDataSeries::next_ubyte()
{

    if (data != NULL && iter < length - 1)
    {
        iter += 1;
        return data[iter];
    }
    else
        return 0;
}

unsigned short OnionDataSeries::next_ushort()
{
    if (data != NULL && iter < length - 2)
    {
        iter += 2;
        return data[iter]|data[iter-1]<<8;
    }
    else
        return 0;
}

unsigned long OnionDataSeries::next_ulong()
{
    if (data != NULL && iter < length - 4)
    {
        iter += 4;
        return data[iter]|data[iter-1]<<8|data[iter-2]<<16|data[iter-3]<<24;
    }
    else
        return 0;
}

float OnionDataSeries::next_float()
{
    if (data != NULL && iter < length - 4)
    {
        iter += 4;

        #pragma warning( suppress : 4244 )
        float temp;

        *((unsigned char*)(&temp) + 3) = data[iter - 3];
        *((unsigned char*)(&temp) + 2) = data[iter - 2];
        *((unsigned char*)(&temp) + 1) = data[iter - 1];
        *((unsigned char*)(&temp) + 0) = data[iter];

        return temp;
    }
    else
        return 0;
}

bool OnionDataSeries::next_bool()
{
    if (data != NULL && iter < length - 1)
    {
        iter += 1;
        return data[iter] > 0;
    }
    else
        return false;
}

void OnionDataSeries::printContents()
{
    while(!done())
        printf("%X ",next_ubyte());
    reset();
}

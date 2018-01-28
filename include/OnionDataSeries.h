#ifndef ONIONDATASERIES_H
#define ONIONDATASERIES_H

#include <string>

class OnionDataSeries
{
    public:
        static int numInstances;

        OnionDataSeries();
        OnionDataSeries(int len);
        OnionDataSeries(std::string filename);
        OnionDataSeries(unsigned char* dataIn, int dataLength);
        ~OnionDataSeries();

        void load(std::string filename);
        void setByte(int index, unsigned char value);
        OnionDataSeries* segment(int startPos, int endPos);
        void loop(bool shouldLoop) {looping = shouldLoop; };

        int pos() { return iter; };
        void reset();
        void set(int index);
        unsigned char at(int index);
        int size();
        bool done();
        void back(int amount = 1);
        void skip(int amount = 1);

        void printContents();

        //READS LITTLE-ENDIAN
        unsigned char next_ubyte(); //8-bit unsigned int
        unsigned short next_ushort(); //16-bit unsigned int
        unsigned long next_ulong(); //32-bit unsigned int
        float next_float(); //32-bit signed floating point
        bool next_bool(); //boolean value


    private:
        int iter;
        int length;
        unsigned char* data;
        bool looping;
};

#endif // ONIONDATASERIES_H

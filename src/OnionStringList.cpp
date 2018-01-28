#include "OnionStringList.h"

#include <cstdio>

int OnionStringList::numInstances = 0;

OnionStringList::OnionStringList()
{
    ++numInstances;
}
OnionStringList::OnionStringList(std::string filename)
{
    load(filename);
    ++numInstances;
}
OnionStringList::OnionStringList(std::vector<std::string> stringsIn):
    strings(stringsIn)
{
    ++numInstances;
}

OnionStringList::~OnionStringList()
{
    /*for (int i = 0; i < strings.size(); ++i)
    {
        delete[] strings[i].c_str();
    }*/
    --numInstances;
}

void OnionStringList::load(std::string filename)
{
    #pragma warning( suppress : 4996 )
    FILE *file = fopen(filename.c_str(), "rb");

    if (file == NULL)
    {
        printf("Can't find %s. Dunno where it went.\n", filename.c_str());
        return;
    }

    fseek(file, 0, SEEK_END);

    int length = ftell(file);
    char* data = new char[length];

    fseek(file, 0, SEEK_SET);

    fread(data, 1, length, file);
    fclose(file);

    int start, finish;
    for (int i = 0; i < length; ++i)
    {
        if (data[i] == 2)
        {
            start = i;
        }
        else if (data[i] == 3)
        {
            finish = i;

            char* seq = new char[finish - start];
            for (int j = 0; j < finish - start - 1; ++j)
            {
                seq[j] = data[j + start + 1];
            }
            seq[finish - start - 1] = '\0';
            strings.push_back(std::string(seq));
            printf("This little stringy went to %s\n", seq);
            delete[] seq;
        }
    }

    delete[] data;
}

std::string OnionStringList::at(int index)
{
    if (index < 0 || index >= strings.size())
        return "";
    else
        return strings[index];
}

void OnionStringList::printContents()
{
    for (int i = 0; i< strings.size(); ++i)
        printf("%s\n", strings.at(i).c_str());
}

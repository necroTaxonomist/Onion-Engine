#include "OnionTextureList.h"

#include <cstdio>

#define AVERAGE_TEX_HEIGHT

int OnionTextureList::numInstances = 0;
int OnionTextureList::containedData = 0;

OnionTextureList::OnionTextureList()
{
    ++numInstances;
}

OnionTextureList::~OnionTextureList()
{
    printf("TEXTURE LIST DESTRUCTOR STARTED\n");
    for (int i = 0; i < ids.size(); ++i)
    {
        glDeleteTextures(1, &ids[i]);
    }
    for (int r = 0; r < rIds.size(); ++r)
    {
        glDeleteTextures(1, &rIds[r]);
    }
    printf("TEXTURE LIST DESTRUCTOR FINISHED\n");
    --numInstances;
}

GLuint OnionTextureList::get(std::string filename)
{
    int index = findFile(filename);
    if (index >= 0)
    {
        printf("found image \"%s\"\n", filename.c_str());
        return ids.at(index);
    }
    else
    {
        glm::vec2 dims = pngSize(filename);

        if (dims.x == 0 && dims.y == 0)
        {
            printf("could not find \"%s\"\n", filename.c_str());
            return NULL;
        }

        printf("loading image \"%s\"\n", filename.c_str());
        GLuint newId;
        if (rIds.size() > 0)
        {
            //printf("reusing ID\n");
            newId = rIds.at(rIds.size() - 1);
            rIds.pop_back();
        }
        else
        {
            //printf("using new ID\n");
            glGenTextures(1, &newId);
        }

        glBindTexture(GL_TEXTURE_2D, newId);


        int channels = 4;
        int fileSize = (int)dims.x * (int)dims.y * channels * sizeof(char);

        int ed, edd, eddy;
        unsigned char* data = SOIL_load_image(filename.c_str(), &ed, &edd, &eddy, SOIL_LOAD_AUTO);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)dims.x, (int)dims.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        SOIL_free_image_data(data);

        //printf("nwidth=%d, height=%d, channels=%d\ntotal=%d\n",
               //(int)dims.x, (int)dims.y, channels, fileSize);

        //printf("ID = %d\n", newId);
        ids.push_back(newId);
        filenames.push_back(filename);
        sizes.push_back(fileSize);
        containedData += fileSize;
        return newId;
    }
}

void OnionTextureList::dealloc(std::string filename)
{
    int index = findFile(filename);
    if (index != -1)
    {
        rIds.push_back(ids.at(index));
        ids.erase(ids.begin()+index);
        filenames.erase(filenames.begin()+index);
        containedData -= sizes.at(index);
        sizes.erase(sizes.begin()+index);
    }
}

void OnionTextureList::dealloc(GLuint id)
{
    int index = findID(id);
    if (index != -1)
    {
        rIds.push_back(ids.at(index));
        ids.erase(ids.begin()+index);
        filenames.erase(filenames.begin()+index);
        containedData -= sizes.at(index);
        sizes.erase(sizes.begin()+index);
    }
}

int OnionTextureList::findFile(std::string filename)
{
    for (int i = 0; i < filenames.size(); ++i)
    {
        if (filename.compare(filenames.at(i)) == 0)
            return i;
    }
    return -1;
}

int OnionTextureList::findID(GLuint id)
{
    for (int i = 0; i < ids.size(); ++i)
    {
        if (ids.at(i) == id)
            return i;
    }
    return -1;
}

glm::vec2 OnionTextureList::pngSize(std::string filename)
{
    printf("HET");

    #pragma warning( suppress : 4996 )
    FILE* f = fopen(filename.c_str(), "rb");
    if (f == NULL)
    {
        return glm::vec2(0,0);
    }
    unsigned char* header = new unsigned char[33];

    fread(header, 1, 33, f);

    int width = header[16]<<24|header[17]<<16|header[18]<<8|header[19];
    int height = header[20]<<24|header[21]<<16|header[22]<<8|header[23];

    fclose(f);

    delete header;

    return glm::vec2(width,height);
}

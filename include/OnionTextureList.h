#ifndef ONIONTEXTURELIST_H
#define ONIONTEXTURELIST_H

#include "OnionBase.h"
#include <SOIL.h>

class OnionTextureList
{
    public:
        static int numInstances;
        static int containedData;

        OnionTextureList();
        ~OnionTextureList();

        GLuint get(std::string filename);
        void dealloc(std::string filename);
        void dealloc(GLuint id);

        glm::vec2 pngSize(std::string filename);

    private:
        std::vector<GLuint> ids;
        std::vector<std::string> filenames;
        std::vector<GLuint> rIds;
        std::vector<int> sizes;

        int findFile(std::string filename);
        int findID(GLuint id);
};

#endif // ONIONTEXTURELIST_H

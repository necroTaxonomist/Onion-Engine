#ifndef ONIONBUFFER_H
#define ONIONBUFFER_H

#include "OnionBase.h"

class OnionBuffer
{
    public:
        static int numInstances;

        OnionBuffer(int numAttribs);
        ~OnionBuffer();

        void defineAttrib(  GLuint index,
                            GLint size = 3,
                            GLboolean normalized = GL_FALSE,
                            GLsizei stride = 0,
                            const GLvoid * pointer = (void*)0);

        void render(GLuint program=0, glm::mat4 mvpMatrix = glm::mat4(1.0f));

        void clear();

        void setDrawTexture(GLuint texture); //CALL BEFORE DRAW

        void setBackground(GLuint newTexture);

        void draw(GLenum drawType, int numVertices, GLfloat arr0[]);
        void draw(GLenum drawType, int numVertices, GLfloat arr0[], GLfloat arr1[]);

    private:
        GLuint vao;
        GLuint* vbos;
        int numBuffers;
        std::vector<GLfloat>* vertexLists;

        std::vector<int> shapeIndices;
        std::vector<GLenum> shapeTypes;
        std::vector<GLuint> shapeTextures;
        int vBufferSize;

        int* sizes;

        GLuint bgTexture;

        void loadVertices();

        //bool verboseDraw;
        //bool verboseRender;
};

#endif // ONIONBUFFER_H

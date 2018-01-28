#include "OnionBuffer.h"

#define MVP_MATRIX_NAME "MVP"
#define TEXTURE_1_NAME "textureSampler"
#define BACKGROUND_NAME "backgroundTexture"

int OnionBuffer::numInstances = 0;

OnionBuffer::OnionBuffer(int numAttribs):
    numBuffers(numAttribs)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vbos = new GLuint[numAttribs];
    glGenBuffers(numAttribs, vbos);

    //printf("What in the %d\n", numBuffers);

    vertexLists = new std::vector<GLfloat>[numAttribs];

    sizes = new int[numAttribs];

    glBindVertexArray(0);

    vBufferSize = 0;

    bgTexture = 0;

    ++numInstances;
}

OnionBuffer::~OnionBuffer()
{
    printf("BUFFER DESTRUCTOR STARTED\n");
    for (int b = 0; b < numBuffers; ++b)
    {
        glDeleteBuffers(1, &vbos[b]);
    }
    glDeleteVertexArrays(1, &vao);
    delete[] vbos;
    delete[] vertexLists;
    delete[] sizes;
    printf("BUFFER DESTRUCTOR FINISHED\n");

    --numInstances;
}

void OnionBuffer::defineAttrib(GLuint index,GLint size,GLboolean normalized,GLsizei stride,const GLvoid* pointer)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[index]);
    glVertexAttribPointer(
        index,
        size,
        GL_FLOAT,
        normalized,
        stride,
        pointer
    );
    sizes[index] = size;
    glBindVertexArray(0);
}

void OnionBuffer::render(GLuint program, glm::mat4 mvpMatrix)
{
    glBindVertexArray(vao);

    for (int i = 0; i < numBuffers; ++i)
    {
        //printf("Enabled %d\n",i);
        glEnableVertexAttribArray(i);
    }

    if (program != 0)
    {
        //printf("The program is %d\n", program);
        glUseProgram(program);

        int matrixID = glGetUniformLocation(program, MVP_MATRIX_NAME);
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvpMatrix[0][0]);

        int bgID;
        if (bgTexture != 0)
        {
            bgID = glGetUniformLocation(program, BACKGROUND_NAME);
            glUniform1i(bgID, 0);
        }

        int textureID;
        if (shapeTextures.size() > 0)
        {
           textureID = glGetUniformLocation(program, TEXTURE_1_NAME);
           glUniform1i(textureID, 1);
        }
    }

    loadVertices();

    //printf("Made it to %d\n",3);

    //printf("The buffer size is %d\n",vBufferSize);

    for (int i = 0; i < shapeIndices.size(); ++i)
    {
        //printf("This time it's %d\n", i);
        if (bgTexture != 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, bgTexture);
        }

        if (shapeTextures.size() > 0)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, shapeTextures.at(i));
        }

        if (i == shapeIndices.size() - 1)
        {
            glDrawArrays(shapeTypes.at(i),
                     shapeIndices.at(i),
                     vBufferSize - shapeIndices.at(i));
        }
        else
        {
            glDrawArrays(shapeTypes.at(i),
                     shapeIndices.at(i),
                     shapeIndices.at(i+1) - shapeIndices.at(i));
        }
    }

    //printf("Made it to %d\n",4);

    for (int j = 0; j < numBuffers; ++j)
    {
        glDisableVertexAttribArray(j);
    }
    glBindVertexArray(0);

    //printf("Made it to %d\n",5);

    clear();
}

void OnionBuffer::loadVertices()
{
    //printf("Made it to %d\n",0);
    int longestList = 0;

    GLfloat** tempArray = new GLfloat*[numBuffers];

    for (int b = 0; b < numBuffers; ++b)
    {

        #pragma warning( suppress : 4267 )
        int length = vertexLists[b].size();

        tempArray[b] = new GLfloat[length];

        if (length > longestList)
            longestList = length;
    }

    //printf("Made it to %d\n",1);

    for (int i = 0; i < longestList; ++i)
    {
        for (int b = 0; b < numBuffers; ++b)
        {
            if (i < vBufferSize * sizes[b])
            {
                tempArray[b][i] = vertexLists[b].at(i);
                //printf("%f,\n", tempArray[b][i]);
            }
        }
    }

    for (int b = 0; b < numBuffers; ++b)
    {
        //printf("The VBO is %d and the size is %d\n", vbos[b], vBufferSize * sizes[b]);
        //printf("%d\n", vBufferSize * sizes[b] * sizeof(GLfloat));
        glBindBuffer(GL_ARRAY_BUFFER, vbos[b]);
        //printf("Made it to %f\n",.5);
        glBufferData(GL_ARRAY_BUFFER, vBufferSize * sizes[b] * sizeof(GLfloat), tempArray[b], GL_DYNAMIC_DRAW);
        //printf("Made it to %d\n",1);
        delete[] tempArray[b];
    }
    delete[] tempArray;

    //printf("Made it to %d\n",2);
}

void OnionBuffer::clear()
{
    for (int b = 0; b < numBuffers; ++b)
    {
        vertexLists[b].clear();
        vBufferSize = 0;
    }
    shapeIndices.clear();
    shapeTypes.clear();
    shapeTextures.clear();
}

void OnionBuffer::setBackground(GLuint newTexture)
{
    bgTexture = newTexture;
}

void OnionBuffer::setDrawTexture(GLuint texture)
{
    shapeTextures.push_back(texture);
}

void OnionBuffer::draw(GLenum drawType, int numVertices, GLfloat arr0[])
{
    shapeIndices.push_back(vBufferSize);
    shapeTypes.push_back(drawType);
    vBufferSize += numVertices;

    for (int i = 0; i < numVertices * sizes[0]; ++i)
    {
        vertexLists[0].push_back(arr0[i]);
    }

    for (int b = 1; b < numBuffers; ++b)
    {
        for (int i = 0; i < numVertices * sizes[b]; ++i)
        {
            vertexLists[b].push_back(0.0f);
        }
    }
}

void OnionBuffer::draw(GLenum drawType, int numVertices, GLfloat arr0[], GLfloat arr1[])
{
    shapeIndices.push_back(vBufferSize);
    shapeTypes.push_back(drawType);
    vBufferSize += numVertices;

    for (int i = 0; i < numVertices * sizes[0]; ++i)
    {
        vertexLists[0].push_back(arr0[i]);
    }

    //printf("Made it to %f\n",3.14);

    for (int i = 0; i < numVertices * sizes[1]; ++i)
    {
        vertexLists[1].push_back(arr1[i]);
    }

    //printf("Made it to %f\n",6.28);

    for (int b = 2; b < numBuffers; ++b)
    {
        for (int i = 0; i < numVertices * sizes[b]; ++i)
        {
            vertexLists[b].push_back(0.0f);
        }
    }

    //printf("%fDREW IT!\n", numVertices);
}
